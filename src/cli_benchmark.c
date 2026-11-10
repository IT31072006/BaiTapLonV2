#include "common.h"
#include "network_producer.h"
#include "storage_consumer.h"
#if __has_include("curl/curl.h")
#include "curl/curl.h"
#else
#include <curl/curl.h>
#endif

int main(int argc, char **argv) {
  SyncMode mode_to_test = SYNC_MODE_SYNCHRONIZED;
  if (argc > 1 && strcmp(argv[1], "--race") == 0) {
    mode_to_test = SYNC_MODE_UNSYNCHRONIZED;
  }

  printf("=================================================================\n");
  printf("LINUX SYSTEM PROGRAMMING: PRODUCER-CONSUMER DOWNLOAD ENGINE\n");
  printf("TEST MODE: %s\n", (mode_to_test == SYNC_MODE_SYNCHRONIZED)
                                ? "SYNCHRONIZED (MUTEX/SEMAPHORE)"
                                : "UNSYNCHRONIZED (RACE CONDITION)");
  printf("=================================================================\n");

  curl_global_init(CURL_GLOBAL_DEFAULT);

  AppContext ctx;
  app_context_init(&ctx);
  ctx.sync_mode = mode_to_test;
  ctx.num_producers = 4;
  ctx.num_consumers = 2;

  /* 1. Generate 16MB (256 chunks) synthetic file */
  const char *source_file = "test_data/benchmark_src.bin";
  const char *dest_file = (mode_to_test == SYNC_MODE_SYNCHRONIZED)
                              ? "test_data/output_sync.bin"
                              : "test_data/output_unsync.bin";
  size_t file_size = 256 * CHUNK_SIZE; /* 16,777,216 bytes */

  printf("[1/5] Generating 16 MB benchmark source file...\n");
  if (generate_benchmark_file(source_file, file_size, ctx.expected_sha256) !=
      0) {
    fprintf(stderr, "Failed to generate benchmark file\n");
    return 1;
  }
  printf("      Source SHA-256: %s\n", ctx.expected_sha256);

  /* 2. Start embedded HTTP Range loopback server */
  printf("[2/5] Starting embedded loopback HTTP server...\n");
  int port = 0;
  if (start_local_http_server(&ctx, source_file, &port) != 0) {
    fprintf(stderr, "Failed to launch embedded server\n");
    return 1;
  }

  /* 3. Probe target with libcurl */
  printf("[3/5] Probing loopback URL via libcurl...\n");
  if (probe_remote_target(&ctx, ctx.target_url) != 0) {
    fprintf(stderr, "Failed to probe remote target\n");
    return 1;
  }

  /* 4. Prepare storage container */
  printf("[4/5] Preallocating storage output container...\n");
  if (prepare_output_file(&ctx, dest_file) != 0) {
    fprintf(stderr, "Failed to prepare output file\n");
    return 1;
  }

  /* 5. Launch Producer and Consumer Pools */
  printf("[5/5] Launching %d Producers and %d Consumers...\n",
         ctx.num_producers, ctx.num_consumers);
  clock_gettime(CLOCK_MONOTONIC, &ctx.start_time);
  ctx.state = STATE_RUNNING;

  start_consumer_pool(&ctx);
  start_producer_pool(&ctx);

  wait_producer_pool(&ctx);
  wait_consumer_pool(&ctx);

  clock_gettime(CLOCK_MONOTONIC, &ctx.end_time);
  ctx.state = STATE_COMPLETED;

  double elapsed = (ctx.end_time.tv_sec - ctx.start_time.tv_sec) +
                   (ctx.end_time.tv_nsec - ctx.start_time.tv_nsec) / 1e9;
  double speed_mbps =
      (ctx.bytes_written / (1024.0 * 1024.0)) / (elapsed > 0 ? elapsed : 0.001);

  printf(
      "\n=================================================================\n");
  printf("TRANSFER TELEMETRY REPORT\n");
  printf("=================================================================\n");
  printf("Elapsed Time         : %.3f seconds\n", elapsed);
  printf("Throughput           : %.2f MB/s\n", speed_mbps);
  printf("Mutex Contentions    : %lu\n", (unsigned long)ctx.mutex_contentions);
  printf("Race Collision Blocks: %lu\n",
         (unsigned long)ctx.race_corrupted_blocks);
  printf("Bytes Downloaded     : %lu / %zu\n",
         (unsigned long)ctx.bytes_downloaded, ctx.total_file_size);
  printf("Bytes Written        : %lu / %zu\n", (unsigned long)ctx.bytes_written,
         ctx.total_file_size);

  /* Verify Cryptographic Integrity */
  printf("\n[VALIDATION] Computing destination SHA-256 digest...\n");
  int verify_result = verify_file_integrity(&ctx);

  printf("EXPECTED: %s\n", ctx.expected_sha256);
  printf("ACTUAL  : %s\n", ctx.actual_sha256);
  if (verify_result == 0) {
    printf("RESULT  : [STATUS: VERIFIED] Byte-to-byte integrity confirmed!\n");
  } else {
    printf("RESULT  : [STATUS: CORRUPTED - DATA RACE DETECTED] Race condition "
           "proved!\n");
  }
  printf("=================================================================\n");

  stop_local_http_server(&ctx);
  app_context_destroy(&ctx);
  curl_global_cleanup();

  return 0;
}
