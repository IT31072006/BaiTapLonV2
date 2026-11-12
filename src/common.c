#include "common.h"
#include <openssl/evp.h>
#include <stdarg.h>

void app_context_init(AppContext *ctx) {
  if (!ctx)
    return;
  memset(ctx, 0, sizeof(AppContext));

  pthread_mutex_init(&ctx->buffer_mutex, NULL);
  pthread_mutex_init(&ctx->file_io_mutex, NULL);
  pthread_mutex_init(&ctx->control_mutex, NULL);
  pthread_mutex_init(&ctx->log_mutex, NULL);
  pthread_cond_init(&ctx->pause_cond, NULL);

  sem_init(&ctx->empty_slots, 0, BUFFER_CAPACITY);
  sem_init(&ctx->full_slots, 0, 0);

  ctx->sync_mode = SYNC_MODE_SYNCHRONIZED;
  ctx->state = STATE_IDLE;
  ctx->is_paused = false;
  ctx->stop_requested = false;

  ctx->num_producers = 4;
  ctx->num_consumers = 2;
  ctx->total_chunks = DEFAULT_TEST_CHUNKS;
  ctx->total_file_size = (size_t)ctx->total_chunks * CHUNK_SIZE;
  ctx->output_fd = -1;
  ctx->local_server_port = 0;
  ctx->local_server_active = false;
  ctx->probe_rtt_ms = 0.0;
  ctx->auto_tune_enabled = true;

  for (int i = 0; i < MAX_CHUNKS; i++) {
    ctx->block_statuses[i] = BLOCK_UNASSIGNED;
    ctx->block_producer_id[i] = -1;
  }
}

void app_context_destroy(AppContext *ctx) {
  if (!ctx)
    return;
  pthread_mutex_destroy(&ctx->buffer_mutex);
  pthread_mutex_destroy(&ctx->file_io_mutex);
  pthread_mutex_destroy(&ctx->control_mutex);
  pthread_mutex_destroy(&ctx->log_mutex);
  pthread_cond_destroy(&ctx->pause_cond);

  sem_destroy(&ctx->empty_slots);
  sem_destroy(&ctx->full_slots);

  if (ctx->output_fd >= 0) {
    close(ctx->output_fd);
    ctx->output_fd = -1;
  }
}

void app_context_reset(AppContext *ctx) {
  if (!ctx)
    return;
  pthread_mutex_lock(&ctx->control_mutex);

  ctx->state = STATE_IDLE;
  ctx->is_paused = false;
  ctx->stop_requested = false;
  ctx->next_chunk_to_produce = 0;
  ctx->chunks_written = 0;
  ctx->bytes_downloaded = 0;
  ctx->bytes_written = 0;
  ctx->mutex_contentions = 0;
  ctx->race_corrupted_blocks = 0;
  ctx->transfer_rate_mbps = 0.0;
  ctx->sha256_verified = false;
  memset(ctx->actual_sha256, 0, sizeof(ctx->actual_sha256));

  /* Reset circular buffer indices */
  pthread_mutex_lock(&ctx->buffer_mutex);
  ctx->buffer.head = 0;
  ctx->buffer.tail = 0;
  ctx->buffer.count = 0;
  for (int i = 0; i < BUFFER_CAPACITY; i++) {
    ctx->buffer.slots[i].is_occupied = false;
    ctx->buffer.slots[i].chunk_id = -1;
  }
  pthread_mutex_unlock(&ctx->buffer_mutex);

  /* Reinitialize POSIX semaphores to default counts */
  sem_destroy(&ctx->empty_slots);
  sem_destroy(&ctx->full_slots);
  sem_init(&ctx->empty_slots, 0, BUFFER_CAPACITY);
  sem_init(&ctx->full_slots, 0, 0);

  /* Reset visualization matrix */
  for (int i = 0; i < MAX_CHUNKS; i++) {
    ctx->block_statuses[i] = BLOCK_UNASSIGNED;
    ctx->block_producer_id[i] = -1;
  }

  if (ctx->output_fd >= 0) {
    close(ctx->output_fd);
    ctx->output_fd = -1;
  }

  pthread_mutex_unlock(&ctx->control_mutex);
}

void app_log(AppContext *ctx, const char *thread_tag, const char *action,
             const char *fmt, ...) {
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  struct tm tm_buf;
  localtime_r(&ts.tv_sec, &tm_buf);

  char time_str[32];
  snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d.%03ld", tm_buf.tm_hour,
           tm_buf.tm_min, tm_buf.tm_sec, ts.tv_nsec / 1000000);

  char detail_msg[160];
  va_list args;
  va_start(args, fmt);
  vsnprintf(detail_msg, sizeof(detail_msg), fmt, args);
  va_end(args);

  char formatted_line[LOG_LINE_MAX_LEN];
  snprintf(formatted_line, sizeof(formatted_line), "%s | %-11s | %-8s | %s",
           time_str, thread_tag, action, detail_msg);

  /* Output to stdout for terminal audit trail */
  printf("%s\n", formatted_line);
  fflush(stdout);

  /* Store inside thread-safe ring buffer for GUI consumption */
  if (ctx) {
    pthread_mutex_lock(&ctx->log_mutex);
    int target_slot = (ctx->log_head + ctx->log_count) % LOG_BUFFER_CAPACITY;
    if (ctx->log_count == LOG_BUFFER_CAPACITY) {
      /* Ring full: displace oldest entry */
      ctx->log_head = (ctx->log_head + 1) % LOG_BUFFER_CAPACITY;
    } else {
      ctx->log_count++;
    }
    size_t slen = strlen(formatted_line);
    if (slen >= LOG_LINE_MAX_LEN)
      slen = LOG_LINE_MAX_LEN - 1;
    memcpy(ctx->log_messages[target_slot], formatted_line, slen);
    ctx->log_messages[target_slot][slen] = '\0';
    pthread_mutex_unlock(&ctx->log_mutex);
  }
}

int compute_file_sha256(const char *filepath, char *out_hash_hex) {
  if (!filepath || !out_hash_hex)
    return -1;

  FILE *fp = fopen(filepath, "rb");
  if (!fp)
    return -1;

  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  if (!mdctx) {
    fclose(fp);
    return -1;
  }

  if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) {
    EVP_MD_CTX_free(mdctx);
    fclose(fp);
    return -1;
  }

  uint8_t buffer[65536];
  size_t bytes_read;
  while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
    if (1 != EVP_DigestUpdate(mdctx, buffer, bytes_read)) {
      EVP_MD_CTX_free(mdctx);
      fclose(fp);
      return -1;
    }
  }

  uint8_t hash[EVP_MAX_MD_SIZE];
  unsigned int hash_len = 0;
  if (1 != EVP_DigestFinal_ex(mdctx, hash, &hash_len)) {
    EVP_MD_CTX_free(mdctx);
    fclose(fp);
    return -1;
  }

  EVP_MD_CTX_free(mdctx);
  fclose(fp);

  for (unsigned int i = 0; i < hash_len; i++) {
    sprintf(out_hash_hex + (i * 2), "%02x", hash[i]);
  }
  out_hash_hex[hash_len * 2] = '\0';

  return 0;
}
