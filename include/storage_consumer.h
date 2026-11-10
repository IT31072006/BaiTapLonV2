#ifndef STORAGE_CONSUMER_H
#define STORAGE_CONSUMER_H

#include "common.h"

/* Consumer thread argument */
typedef struct {
    AppContext *ctx;
    int consumer_id;
} ConsumerArg;

/* =========================================================================
 * STORAGE CONSUMER API
 * ========================================================================= */

/* Prepares output file descriptor with pre-allocated size */
int prepare_output_file(AppContext *ctx, const char *dest_path);

/* Consumer worker thread routine */
void* consumer_worker(void *arg);

/* Launches consumer thread pool */
int start_consumer_pool(AppContext *ctx);

/* Waits for all consumer threads to terminate */
void wait_consumer_pool(AppContext *ctx);

/* Computes SHA-256 of completed file and validates against expected digest */
int verify_file_integrity(AppContext *ctx);

#endif /* STORAGE_CONSUMER_H */

