#include "network_producer.h"
#include <arpa/inet.h>
#if __has_include("curl/curl.h")
#include "curl/curl.h"
#else
#include <curl/curl.h>
#endif
#include <netdb.h>
#include <netinet/in.h>
#include <openssl/evp.h>
#include <sys/socket.h>

/* Structure for local HTTP server background thread */
typedef struct {
  AppContext *ctx;
  int server_sock;
  char filepath[512];
  pthread_t thread;
  bool running;
} LocalServerState;

static LocalServerState g_local_server = {0};

/* =========================================================================
 * BENCHMARK SYNTHETIC FILE GENERATOR
 * Generates deterministic 64KB-aligned binary payload for 100% offline tests
 * ========================================================================= */
int generate_benchmark_file(const char *filepath, size_t size_bytes,
                            char *out_sha256) {
  if (!filepath || size_bytes == 0)
    return -1;

  int fd = open(filepath, O_CREAT | O_WRONLY | O_TRUNC, 0644);
  if (fd < 0) {
    perror("open benchmark file");
    return -1;
  }

  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  if (!mdctx || 1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) {
    if (mdctx)
      EVP_MD_CTX_free(mdctx);
    close(fd);
    return -1;
  }

  uint8_t *chunk_buf = malloc(CHUNK_SIZE);
  if (!chunk_buf) {
    EVP_MD_CTX_free(mdctx);
    close(fd);
    return -1;
  }

  size_t total_chunks = (size_bytes + CHUNK_SIZE - 1) / CHUNK_SIZE;
  size_t bytes_remaining = size_bytes;

  for (size_t chunk_idx = 0; chunk_idx < total_chunks; chunk_idx++) {
    size_t current_chunk_size =
        (bytes_remaining > CHUNK_SIZE) ? CHUNK_SIZE : bytes_remaining;

    /* Deterministic high-entropy pattern:
     * Byte 0..3: Chunk ID
     * Byte 4..11: Byte offset
     * Byte 12..15: Technical magic 0x53595350 ("SYSP")
     * Byte 16..N: Deterministic pseudorandom sequence
     */
    uint32_t cid = (uint32_t)chunk_idx;
    uint64_t off = (uint64_t)(chunk_idx * CHUNK_SIZE);
    uint32_t magic = 0x53595350;

    memcpy(&chunk_buf[0], &cid, 4);
    memcpy(&chunk_buf[4], &off, 8);
    memcpy(&chunk_buf[12], &magic, 4);

    for (size_t i = 16; i < current_chunk_size; i++) {
      chunk_buf[i] = (uint8_t)(((i * 109 + chunk_idx * 199) ^ 0xA5) & 0xFF);
    }

    if (write(fd, chunk_buf, current_chunk_size) !=
        (ssize_t)current_chunk_size) {
      free(chunk_buf);
      EVP_MD_CTX_free(mdctx);
      close(fd);
      return -1;
    }

    EVP_DigestUpdate(mdctx, chunk_buf, current_chunk_size);
    bytes_remaining -= current_chunk_size;
  }

  free(chunk_buf);
  close(fd);

  uint8_t hash[EVP_MAX_MD_SIZE];
  unsigned int hash_len = 0;
  EVP_DigestFinal_ex(mdctx, hash, &hash_len);
  EVP_MD_CTX_free(mdctx);

  if (out_sha256) {
    for (unsigned int i = 0; i < hash_len; i++) {
      sprintf(out_sha256 + (i * 2), "%02x", hash[i]);
    }
    out_sha256[hash_len * 2] = '\0';
  }

  return 0;
}

/* =========================================================================
 * EMBEDDED LOCAL HTTP SERVER (100% OFFLINE HTTP RANGE RFC COMPLIANT)
 * Allows libcurl to perform real multi-threaded HTTP Range requests over
 * 127.0.0.1
 * ========================================================================= */
static void *local_http_server_routine(void *arg) {
  (void)arg;
  int server_fd = g_local_server.server_sock;

  while (g_local_server.running) {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(server_fd, &read_fds);

    struct timeval tv = {.tv_sec = 0, .tv_usec = 200000}; /* 200ms timeout */
    int sel = select(server_fd + 1, &read_fds, NULL, NULL, &tv);
    if (sel <= 0)
      continue;

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sock =
        accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock < 0)
      continue;

    /* Read HTTP request header */
    char req_buf[2048];
    ssize_t rlen = recv(client_sock, req_buf, sizeof(req_buf) - 1, 0);
    if (rlen <= 0) {
      close(client_sock);
      continue;
    }
    req_buf[rlen] = '\0';

    /* Inspect file */
    struct stat st;
    if (stat(g_local_server.filepath, &st) < 0) {
      const char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: "
                              "0\r\nConnection: close\r\n\r\n";
      send(client_sock, not_found, strlen(not_found), 0);
      close(client_sock);
      continue;
    }

    bool is_head = (strncmp(req_buf, "HEAD", 4) == 0);
    char *range_header = strstr(req_buf, "Range: bytes=");

    if (is_head) {
      char resp[512];
      snprintf(resp, sizeof(resp),
               "HTTP/1.1 200 OK\r\n"
               "Server: LinuxNativeWorkstation/1.0\r\n"
               "Content-Length: %zu\r\n"
               "Accept-Ranges: bytes\r\n"
               "Content-Type: application/octet-stream\r\n"
               "Connection: close\r\n\r\n",
               (size_t)st.st_size);
      send(client_sock, resp, strlen(resp), 0);
    } else if (range_header) {
      /* Range: bytes=start-end */
      off_t r_start = 0;
      off_t r_end = st.st_size - 1;
      sscanf(range_header + 13, "%ld-%ld", &r_start, &r_end);
      if (r_end >= st.st_size)
        r_end = st.st_size - 1;
      size_t r_len = (size_t)(r_end - r_start + 1);

      char resp[512];
      snprintf(resp, sizeof(resp),
               "HTTP/1.1 206 Partial Content\r\n"
               "Server: LinuxNativeWorkstation/1.0\r\n"
               "Content-Range: bytes %ld-%ld/%zu\r\n"
               "Content-Length: %zu\r\n"
               "Accept-Ranges: bytes\r\n"
               "Content-Type: application/octet-stream\r\n"
               "Connection: close\r\n\r\n",
               (long)r_start, (long)r_end, (size_t)st.st_size, r_len);
      send(client_sock, resp, strlen(resp), 0);

      /* Send binary range payload */
      int file_fd = open(g_local_server.filepath, O_RDONLY);
      if (file_fd >= 0) {
        lseek(file_fd, r_start, SEEK_SET);
        uint8_t stream_buf[16384];
        size_t remaining = r_len;
        while (remaining > 0) {
          size_t to_read =
              (remaining > sizeof(stream_buf)) ? sizeof(stream_buf) : remaining;
          ssize_t n_read = read(file_fd, stream_buf, to_read);
          if (n_read <= 0)
            break;
          send(client_sock, stream_buf, n_read, 0);
          remaining -= n_read;
        }
        close(file_fd);
      }
    } else {
      /* Standard GET without range */
      char resp[512];
      snprintf(resp, sizeof(resp),
               "HTTP/1.1 200 OK\r\n"
               "Server: LinuxNativeWorkstation/1.0\r\n"
               "Content-Length: %zu\r\n"
               "Accept-Ranges: bytes\r\n"
               "Content-Type: application/octet-stream\r\n"
               "Connection: close\r\n\r\n",
               (size_t)st.st_size);
      send(client_sock, resp, strlen(resp), 0);

      int file_fd = open(g_local_server.filepath, O_RDONLY);
      if (file_fd >= 0) {
        uint8_t stream_buf[16384];
        ssize_t n_read;
        while ((n_read = read(file_fd, stream_buf, sizeof(stream_buf))) > 0) {
          send(client_sock, stream_buf, n_read, 0);
        }
        close(file_fd);
      }
    }

    close(client_sock);
  }

  return NULL;
}

int start_local_http_server(AppContext *ctx, const char *filepath,
                            int *out_port) {
  if (g_local_server.running) {
    stop_local_http_server(ctx);
  }

  int s_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (s_sock < 0)
    return -1;

  int opt = 1;
  setsockopt(s_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = 0; /* Ephemeral port assigned by Linux Kernel */

  if (bind(s_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    close(s_sock);
    return -1;
  }

  if (listen(s_sock, 16) < 0) {
    close(s_sock);
    return -1;
  }

  socklen_t addr_len = sizeof(serv_addr);
  getsockname(s_sock, (struct sockaddr *)&serv_addr, &addr_len);
  int assigned_port = ntohs(serv_addr.sin_port);

  g_local_server.ctx = ctx;
  g_local_server.server_sock = s_sock;
  strncpy(g_local_server.filepath, filepath,
          sizeof(g_local_server.filepath) - 1);
  g_local_server.running = true;

  pthread_create(&g_local_server.thread, NULL, local_http_server_routine, NULL);

  if (out_port)
    *out_port = assigned_port;
  if (ctx) {
    ctx->local_server_active = true;
    ctx->local_server_port = assigned_port;
    snprintf(ctx->target_url, sizeof(ctx->target_url),
             "http://127.0.0.1:%d/benchmark.bin", assigned_port);
    app_log(ctx, "SYS_ENGINE", "LOCAL_SRV",
            "In-process Range Server listening at 127.0.0.1:%d", assigned_port);
  }

  return 0;
}

void stop_local_http_server(AppContext *ctx) {
  if (!g_local_server.running)
    return;

  g_local_server.running = false;
  pthread_join(g_local_server.thread, NULL);
  close(g_local_server.server_sock);
  g_local_server.server_sock = -1;

  if (ctx) {
    ctx->local_server_active = false;
    app_log(ctx, "SYS_ENGINE", "LOCAL_SRV",
            "In-process Range Server terminated");
  }
}

/* =========================================================================
 * HTTP PROBE & METADATA DISCOVERY VIA LIBCURL
 * ========================================================================= */
typedef struct {
  bool accept_ranges;
} HeaderProbeData;

static size_t probe_header_callback(char *buffer, size_t size, size_t nitems,
                                    void *userdata) {
  size_t total = size * nitems;
  HeaderProbeData *data = (HeaderProbeData *)userdata;
  if (strncasecmp(buffer, "Accept-Ranges:", 14) == 0) {
    if (strcasestr(buffer, "bytes")) {
      data->accept_ranges = true;
    }
  }
  return total;
}

int probe_remote_target(AppContext *ctx, const char *url) {
  if (!ctx || !url)
    return -1;

  CURL *curl = curl_easy_init();
  if (!curl)
    return -1;

  HeaderProbeData probe_data = {.accept_ranges = false};

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, probe_header_callback);
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, &probe_data);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    app_log(ctx, "SYS_ENGINE", "PROBE_ERR", "HTTP HEAD failed: %s",
            curl_easy_strerror(res));
    curl_easy_cleanup(curl);
    return -1;
  }

  curl_off_t cl = 0;
#ifdef CURLINFO_CONTENT_LENGTH_DOWNLOAD_T
  curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &cl);
#else
  double cl_d = 0.0;
  curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &cl_d);
  cl = (curl_off_t)cl_d;
#endif
  long response_code = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
  curl_easy_cleanup(curl);

  if (response_code != 200 && response_code != 206) {
    app_log(ctx, "SYS_ENGINE", "PROBE_ERR", "Target returned HTTP status %ld",
            response_code);
    return -1;
  }

  if (cl <= 0) {
    app_log(ctx, "SYS_ENGINE", "PROBE_ERR",
            "Invalid or missing Content-Length (%ld)", (long)cl);
    return -1;
  }

  ctx->total_file_size = (size_t)cl;
  ctx->total_chunks = (ctx->total_file_size + CHUNK_SIZE - 1) / CHUNK_SIZE;
  if (ctx->total_chunks > MAX_CHUNKS) {
    ctx->total_chunks = MAX_CHUNKS;
    ctx->total_file_size = (size_t)ctx->total_chunks * CHUNK_SIZE;
    app_log(ctx, "SYS_ENGINE", "PROBE_WARN",
            "File truncated to maximum supported %d chunks (%zu bytes)",
            MAX_CHUNKS, ctx->total_file_size);
  }

  app_log(ctx, "SYS_ENGINE", "METADATA",
          "Length=%zu bytes Chunks=%d RangeSupport=%s", ctx->total_file_size,
          ctx->total_chunks,
          probe_data.accept_ranges ? "YES" : "UNKNOWN/ASSUMED");

  return 0;
}

/* =========================================================================
 * PRODUCER WORKER THREAD ENGINE
 * ========================================================================= */
static size_t curl_chunk_write_callback(void *ptr, size_t size, size_t nmemb,
                                        void *userdata) {
  size_t total = size * nmemb;
  struct DataChunk *chunk = (struct DataChunk *)userdata;

  if (chunk->size + total > CHUNK_SIZE) {
    total = CHUNK_SIZE - chunk->size;
  }
  memcpy(chunk->buffer + chunk->size, ptr, total);
  chunk->size += total;
  return size * nmemb;
}

void *producer_worker(void *arg) {
  ProducerArg *parg = (ProducerArg *)arg;
  AppContext *ctx = parg->ctx;
  int pid = parg->producer_id;

  char thread_tag[16];
  snprintf(thread_tag, sizeof(thread_tag), "P_THREAD_%02d", pid);

  CURL *curl = curl_easy_init();
  if (!curl) {
    app_log(ctx, thread_tag, "ERROR", "Failed to initialize CURL handle");
    free(parg);
    return NULL;
  }

  while (!ctx->stop_requested) {
    /* Check Pause Condition */
    pthread_mutex_lock(&ctx->control_mutex);
    while (ctx->is_paused && !ctx->stop_requested) {
      pthread_cond_wait(&ctx->pause_cond, &ctx->control_mutex);
    }
    if (ctx->stop_requested) {
      pthread_mutex_unlock(&ctx->control_mutex);
      break;
    }

    /* Atomically reserve next available chunk */
    int chunk_id = ctx->next_chunk_to_produce++;
    pthread_mutex_unlock(&ctx->control_mutex);

    if (chunk_id >= ctx->total_chunks) {
      /* All chunks have been allocated across producer pool */
      break;
    }

    /* Update block allocation state for Cairo visualization */
    ctx->block_statuses[chunk_id] = BLOCK_DOWNLOADING;
    ctx->block_producer_id[chunk_id] = pid;

    off_t byte_start = (off_t)chunk_id * CHUNK_SIZE;
    off_t byte_end = byte_start + CHUNK_SIZE - 1;
    if (byte_end >= (off_t)ctx->total_file_size) {
      byte_end = ctx->total_file_size - 1;
    }

    struct DataChunk chunk;
    chunk.chunk_id = chunk_id;
    chunk.byte_offset = byte_start;
    chunk.size = 0;
    chunk.producer_id = pid;
    chunk.is_occupied = true;

    /* Execute HTTP Range Request */
    char range_header[64];
    snprintf(range_header, sizeof(range_header), "%ld-%ld", (long)byte_start,
             (long)byte_end);

    curl_easy_setopt(curl, CURLOPT_URL, ctx->target_url);
    curl_easy_setopt(curl, CURLOPT_RANGE, range_header);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_chunk_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      app_log(ctx, thread_tag, "NET_ERR", "Range %s transfer failed: %s",
              range_header, curl_easy_strerror(res));
      continue;
    }

    /* =================================================================
     * BOUNDED BUFFER ENQUEUE: SYNCHRONIZED VS UNSYNCHRONIZED
     * ================================================================= */
    if (ctx->sync_mode == SYNC_MODE_SYNCHRONIZED) {
      /* POSIX Semaphore: Decrement available empty slots */
      sem_wait(&ctx->empty_slots);

      /* Mutex Lock with contention tracking */
      if (pthread_mutex_trylock(&ctx->buffer_mutex) != 0) {
        __sync_fetch_and_add(&ctx->mutex_contentions, 1);
        pthread_mutex_lock(&ctx->buffer_mutex);
      }

      int target_slot = ctx->buffer.head;
      ctx->buffer.slots[target_slot] = chunk;
      ctx->buffer.head = (ctx->buffer.head + 1) % BUFFER_CAPACITY;
      ctx->buffer.count++;

      ctx->block_statuses[chunk_id] = BLOCK_QUEUED;

      int sem_full_val = 0;
      sem_getvalue(&ctx->full_slots, &sem_full_val);
      app_log(ctx, thread_tag, "ENQUEUE",
              "chunk_id=%04d offset=0x%08lX sem_full=%02d", chunk_id,
              (unsigned long)byte_start, sem_full_val + 1);

      pthread_mutex_unlock(&ctx->buffer_mutex);

      /* POSIX Semaphore: Increment occupied slot counter */
      sem_post(&ctx->full_slots);
    } else {
      /* UNSYNCHRONIZED (RACE CONDITION) MODE:
       * Mutex and Semaphore are bypassed. A deliberate preemption gap
       * allows competing producers to read the identical head index,
       * causing direct memory overwrites in the bounded buffer!
       */
      int target_slot = ctx->buffer.head;

      /* Deliberate thread preemption delay (1.5 ms) */
      usleep(1500);

      ctx->buffer.slots[target_slot] = chunk;
      ctx->buffer.head = (target_slot + 1) % BUFFER_CAPACITY;
      ctx->buffer.count++;

      ctx->block_statuses[chunk_id] = BLOCK_QUEUED;
      app_log(ctx, thread_tag, "ENQUEUE_RACE",
              "Unsync push at slot %02d chunk_id=%04d offset=0x%08lX",
              target_slot, chunk_id, (unsigned long)byte_start);
    }

    __sync_fetch_and_add(&ctx->bytes_downloaded, chunk.size);
  }

  curl_easy_cleanup(curl);
  free(parg);
  return NULL;
}

int start_producer_pool(AppContext *ctx) {
  if (!ctx)
    return -1;
  if (ctx->num_producers < 1)
    ctx->num_producers = 1;
  if (ctx->num_producers > MAX_PRODUCER_THREADS)
    ctx->num_producers = MAX_PRODUCER_THREADS;

  for (int i = 0; i < ctx->num_producers; i++) {
    ProducerArg *arg = malloc(sizeof(ProducerArg));
    arg->ctx = ctx;
    arg->producer_id = i + 1;
    pthread_create(&ctx->producer_threads[i], NULL, producer_worker, arg);
  }

  app_log(ctx, "SYS_ENGINE", "POOL_PROD", "Spawned %d network producer threads",
          ctx->num_producers);
  return 0;
}

void wait_producer_pool(AppContext *ctx) {
  if (!ctx)
    return;
  for (int i = 0; i < ctx->num_producers; i++) {
    pthread_join(ctx->producer_threads[i], NULL);
  }
  app_log(ctx, "SYS_ENGINE", "POOL_PROD",
          "All network producer threads joined");
}
