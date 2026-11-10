#include "storage_consumer.h"

int prepare_output_file(AppContext *ctx, const char *dest_path) {
  if (!ctx || !dest_path)
    return -1;

  if (ctx->output_fd >= 0) {
    close(ctx->output_fd);
    ctx->output_fd = -1;
  }

  strncpy(ctx->output_filepath, dest_path, sizeof(ctx->output_filepath) - 1);

  /* Open shared file descriptor with direct write privileges */
  int fd = open(dest_path, O_RDWR | O_CREAT | O_TRUNC, 0644);
  if (fd < 0) {
    app_log(ctx, "SYS_ENGINE", "IO_ERR",
            "Failed to create output file '%s': %s", dest_path,
            strerror(errno));
    return -1;
  }

  /* Pre-allocate sparse storage footprint */
  if (ftruncate(fd, ctx->total_file_size) < 0) {
    app_log(ctx, "SYS_ENGINE", "IO_ERR", "ftruncate to %zu bytes failed: %s",
            ctx->total_file_size, strerror(errno));
    close(fd);
    return -1;
  }

  ctx->output_fd = fd;
  app_log(ctx, "SYS_ENGINE", "STORAGE",
          "Pre-allocated storage container '%s' (%zu bytes, fd=%d)", dest_path,
          ctx->total_file_size, fd);
  return 0;
}

void *consumer_worker(void *arg) {
  ConsumerArg *carg = (ConsumerArg *)arg;
  AppContext *ctx = carg->ctx;
  int cid = carg->consumer_id;

  char thread_tag[16];
  snprintf(thread_tag, sizeof(thread_tag), "C_THREAD_%02d", cid);

  while (!ctx->stop_requested) {
    /* Check Pause Condition */
    pthread_mutex_lock(&ctx->control_mutex);
    while (ctx->is_paused && !ctx->stop_requested) {
      pthread_cond_wait(&ctx->pause_cond, &ctx->control_mutex);
    }
    pthread_mutex_unlock(&ctx->control_mutex);

    /* Exit if complete or stopped */
    if (ctx->stop_requested || ctx->chunks_written >= ctx->total_chunks) {
      break;
    }

    struct DataChunk chunk;
    memset(&chunk, 0, sizeof(struct DataChunk));

    /* =================================================================
     * BOUNDED BUFFER DEQUEUE: SYNCHRONIZED VS UNSYNCHRONIZED
     * ================================================================= */
    if (ctx->sync_mode == SYNC_MODE_SYNCHRONIZED) {
      /* POSIX Semaphore: Decrement occupied slot counter */
      sem_wait(&ctx->full_slots);

      /* Check exit condition upon wakeup */
      if (ctx->stop_requested || ctx->chunks_written >= ctx->total_chunks) {
        sem_post(&ctx->full_slots); /* Cascade wakeup to peer consumers */
        break;
      }

      /* Mutex Lock with contention telemetry */
      if (pthread_mutex_trylock(&ctx->buffer_mutex) != 0) {
        __sync_fetch_and_add(&ctx->mutex_contentions, 1);
        pthread_mutex_lock(&ctx->buffer_mutex);
      }

      int target_tail = ctx->buffer.tail;
      chunk = ctx->buffer.slots[target_tail];
      ctx->buffer.slots[target_tail].is_occupied = false;
      ctx->buffer.tail = (target_tail + 1) % BUFFER_CAPACITY;
      ctx->buffer.count--;

      int sem_empty_val = 0;
      sem_getvalue(&ctx->empty_slots, &sem_empty_val);
      app_log(ctx, thread_tag, "DEQUEUE",
              "chunk_id=%04d offset=0x%08lX sem_empty=%02d", chunk.chunk_id,
              (unsigned long)chunk.byte_offset, sem_empty_val + 1);

      pthread_mutex_unlock(&ctx->buffer_mutex);

      /* POSIX Semaphore: Increment available empty slot counter */
      sem_post(&ctx->empty_slots);

      /* =============================================================
       * SAFE SYNCHRONIZED DISK COMMIT
       * Guarded by dedicated file_io_mutex to guarantee atomicity of
       * lseek + write sequence.
       * ============================================================= */
      if (pthread_mutex_trylock(&ctx->file_io_mutex) != 0) {
        __sync_fetch_and_add(&ctx->mutex_contentions, 1);
        pthread_mutex_lock(&ctx->file_io_mutex);
      }

      lseek(ctx->output_fd, chunk.byte_offset, SEEK_SET);
      ssize_t bytes_written = write(ctx->output_fd, chunk.buffer, chunk.size);

      pthread_mutex_unlock(&ctx->file_io_mutex);

      if (bytes_written == (ssize_t)chunk.size) {
        ctx->block_statuses[chunk.chunk_id] = BLOCK_WRITTEN;
        int written_total = __sync_add_and_fetch(&ctx->chunks_written, 1);
        __sync_fetch_and_add(&ctx->bytes_written, bytes_written);
        app_log(ctx, thread_tag, "COMMIT",
                "chunk_id=%04d offset=0x%08lX bytes=%zu (VERIFIED)",
                chunk.chunk_id, (unsigned long)chunk.byte_offset, chunk.size);

        /* If all chunks completed, cascade wakeup to peer consumers */
        if (written_total >= ctx->total_chunks) {
          sem_post(&ctx->full_slots);
        }
      }
    } else {
      /* =============================================================
       * UNSYNCHRONIZED (RACE CONDITION DEMONSTRATION) MODE
       * ============================================================= */

      /* Step A: Circular Queue Dequeue without lock */
      if (ctx->buffer.count <= 0) {
        if (ctx->chunks_written >= ctx->total_chunks) {
          break;
        }
        usleep(2000);
        continue;
      }

      int target_tail = ctx->buffer.tail;
      /* Intentional preemption delay */
      usleep(1500);

      chunk = ctx->buffer.slots[target_tail];
      ctx->buffer.tail = (target_tail + 1) % BUFFER_CAPACITY;
      ctx->buffer.count--;

      /* Step B: Shared File Descriptor lseek & write Race Condition
       * Multiple consumer threads share the single file descriptor `fd`.
       * Calling lseek, pausing for 2.5ms, then writing WITHOUT file_io_mutex
       * allows an interleaved thread to call lseek to a different offset,
       * causing the write to overwrite an unintended position in the file!
       */
      lseek(ctx->output_fd, chunk.byte_offset, SEEK_SET);

      /* Critical race vulnerability window (2.5 ms) */
      usleep(2500);

      /* Audit actual file descriptor position before dispatching write */
      off_t actual_offset = lseek(ctx->output_fd, 0, SEEK_CUR);

      if (actual_offset != chunk.byte_offset) {
        /* Collision confirmed! Another consumer hijacked the file offset */
        __sync_fetch_and_add(&ctx->race_corrupted_blocks, 1);
        ctx->block_statuses[chunk.chunk_id] = BLOCK_CORRUPTED;

        app_log(ctx, thread_tag, "RACE_ERR",
                "lseek collision detected at offset=0x%08lX (expected 0x%08lX, "
                "actual 0x%08lX)",
                (unsigned long)chunk.byte_offset,
                (unsigned long)chunk.byte_offset, (unsigned long)actual_offset);
      } else {
        ctx->block_statuses[chunk.chunk_id] = BLOCK_WRITTEN;
        app_log(ctx, thread_tag, "COMMIT_UNS",
                "chunk_id=%04d offset=0x%08lX bytes=%zu", chunk.chunk_id,
                (unsigned long)chunk.byte_offset, chunk.size);
      }

      /* Flushes chunk into whatever offset the hijacked file descriptor is at
       */
      ssize_t bytes_written = write(ctx->output_fd, chunk.buffer, chunk.size);
      int written_total = __sync_add_and_fetch(&ctx->chunks_written, 1);
      if (bytes_written > 0) {
        __sync_fetch_and_add(&ctx->bytes_written, bytes_written);
      }

      if (written_total >= ctx->total_chunks) {
        break;
      }
    }
  }

  free(carg);
  return NULL;
}

int start_consumer_pool(AppContext *ctx) {
  if (!ctx)
    return -1;
  if (ctx->num_consumers < 1)
    ctx->num_consumers = 1;
  if (ctx->num_consumers > MAX_CONSUMER_THREADS)
    ctx->num_consumers = MAX_CONSUMER_THREADS;

  for (int i = 0; i < ctx->num_consumers; i++) {
    ConsumerArg *arg = malloc(sizeof(ConsumerArg));
    arg->ctx = ctx;
    arg->consumer_id = i + 1;
    pthread_create(&ctx->consumer_threads[i], NULL, consumer_worker, arg);
  }

  app_log(ctx, "SYS_ENGINE", "POOL_CONS", "Spawned %d storage consumer threads",
          ctx->num_consumers);
  return 0;
}

void wait_consumer_pool(AppContext *ctx) {
  if (!ctx)
    return;
  /* Wake up any sleeping consumers */
  for (int i = 0; i < ctx->num_consumers; i++) {
    sem_post(&ctx->full_slots);
  }
  for (int i = 0; i < ctx->num_consumers; i++) {
    pthread_join(ctx->consumer_threads[i], NULL);
  }
  app_log(ctx, "SYS_ENGINE", "POOL_CONS",
          "All storage consumer threads joined");
}

int verify_file_integrity(AppContext *ctx) {
  if (!ctx)
    return -1;

  /* Flush and sync storage container */
  if (ctx->output_fd >= 0) {
    fsync(ctx->output_fd);
    close(ctx->output_fd);
    ctx->output_fd = -1;
  }

  app_log(ctx, "SYS_ENGINE", "CHECKSUM",
          "Computing SHA-256 for target container '%s'...",
          ctx->output_filepath);

  if (compute_file_sha256(ctx->output_filepath, ctx->actual_sha256) != 0) {
    app_log(ctx, "SYS_ENGINE", "CHECKSUM", "EVP SHA-256 computation failed");
    return -1;
  }

  ctx->sha256_verified =
      (strcmp(ctx->expected_sha256, ctx->actual_sha256) == 0);

  if (ctx->sha256_verified) {
    app_log(ctx, "SYS_ENGINE", "CHECKSUM", "STATUS: VERIFIED [SHA-256 MATCH]");
    app_log(ctx, "SYS_ENGINE", "DIGEST", "EXPECTED: %s", ctx->expected_sha256);
    app_log(ctx, "SYS_ENGINE", "DIGEST", "ACTUAL  : %s", ctx->actual_sha256);
  } else {
    app_log(ctx, "SYS_ENGINE", "CHECKSUM",
            "STATUS: CORRUPTED - DATA RACE DETECTED");
    app_log(ctx, "SYS_ENGINE", "DIGEST", "EXPECTED: %s", ctx->expected_sha256);
    app_log(ctx, "SYS_ENGINE", "DIGEST", "ACTUAL  : %s", ctx->actual_sha256);
  }

  return ctx->sha256_verified ? 0 : 1;
}
