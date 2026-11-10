#ifndef NETWORK_PRODUCER_H
#define NETWORK_PRODUCER_H

#include "common.h"

/* Producer thread argument */
typedef struct {
    AppContext *ctx;
    int producer_id;
} ProducerArg;

/* =========================================================================
 * NETWORK PRODUCER API
 * ========================================================================= */

/* Generates a deterministic high-entropy benchmark file and computes its SHA-256 */
int generate_benchmark_file(const char *filepath, size_t size_bytes, char *out_sha256);

/* Starts lightweight embedded HTTP loopback server for 100% offline range testing */
int start_local_http_server(AppContext *ctx, const char *filepath, int *out_port);

/* Stops local embedded server */
void stop_local_http_server(AppContext *ctx);

/* Inspects remote URL via HTTP HEAD to determine file size and byte-range support */
int probe_remote_target(AppContext *ctx, const char *url);

/* Producer worker thread routine */
void* producer_worker(void *arg);

/* Launches producer thread pool */
int start_producer_pool(AppContext *ctx);

/* Waits for all producer threads to terminate */
void wait_producer_pool(AppContext *ctx);

#endif /* NETWORK_PRODUCER_H */

