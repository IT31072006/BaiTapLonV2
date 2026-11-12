#ifndef COMMON_H
#define COMMON_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>

/* =========================================================================
 * ARCHITECTURAL CONSTANTS & MEMORY GEOMETRY
 * ========================================================================= */
#define CHUNK_SIZE              (64 * 1024)   /* 65,536 Bytes = 64 KB */
#define BUFFER_CAPACITY         32            /* Circular Queue capacity in RAM */
#define MAX_CHUNKS              512           /* Supports up to 32MB with 64KB chunks */
#define DEFAULT_TEST_CHUNKS     256           /* 256 chunks * 64KB = 16 MB matrix */
#define LARGE_TEST_CHUNKS       512           /* 512 chunks * 64KB = 32 MB */
#define MAX_PRODUCER_THREADS    8
#define MAX_CONSUMER_THREADS    8
#define LOG_BUFFER_CAPACITY     128
#define LOG_LINE_MAX_LEN        256

/* =========================================================================
 * COLOR PALETTE (MATTE CARBON & OXIDE - INDUSTRIAL WORKSTATION SPEC)
 * Strictly adheres to the Anti-AI design specifications.
 * ========================================================================= */
#define COLOR_APP_BG            "#141413"     /* Warm Matte Black */
#define COLOR_PANEL_SURFACE     "#1C1C1A"     /* Dark Graphite */
#define COLOR_DEEP_INSET        "#10100F"     /* Deep Inset for entries & logs */
#define COLOR_BORDER_HAIRLINE   "#2C2C29"     /* 1px Solid Technical Border */
#define COLOR_TEXT_PRIMARY      "#E5E5E0"     /* Ivory White */
#define COLOR_TEXT_MUTED        "#85857F"     /* Oxide Gray */

/* Telemetry & Block Semantics */
#define COLOR_BLOCK_UNASSIGNED  "#262624"     /* Matte Slate Gray */
#define COLOR_BLOCK_PRODUCING   "#4A88C7"     /* Steel Blue - Producer active */
#define COLOR_BLOCK_QUEUED      "#D4973B"     /* Warm Ochre - Queued in RAM */
#define COLOR_BLOCK_COMMITTED   "#4E9A67"     /* Sage Forest Green - Safe on Disk */
#define COLOR_BLOCK_CORRUPTED   "#C94A4A"     /* Brick Crimson - Race Condition Error */

/* =========================================================================
 * ENUMERATIONS & STATE DEFINITIONS
 * ========================================================================= */
typedef enum {
    BLOCK_UNASSIGNED = 0,     /* Not yet fetched */
    BLOCK_DOWNLOADING = 1,    /* Producer currently fetching */
    BLOCK_QUEUED = 2,         /* Waiting in Circular Buffer */
    BLOCK_WRITTEN = 3,        /* Flushed to storage safely */
    BLOCK_CORRUPTED = 4       /* Corrupted due to Race Condition */
} BlockStatus;

typedef enum {
    SYNC_MODE_UNSYNCHRONIZED = 0, /* Intentional Race Condition Mode */
    SYNC_MODE_SYNCHRONIZED   = 1  /* POSIX Mutex & Semaphore Guarded Mode */
} SyncMode;

typedef enum {
    STATE_IDLE = 0,
    STATE_RUNNING = 1,
    STATE_PAUSED = 2,
    STATE_COMPLETED = 3,
    STATE_ERROR = 4
} EngineState;

/* =========================================================================
 * DATA STRUCTURES
 * ========================================================================= */

/* Individual transfer unit packed by producers and consumed by storage */
struct DataChunk {
    int chunk_id;                     /* 0-indexed sequence ID */
    off_t byte_offset;                /* Byte position in destination file */
    size_t size;                      /* Chunk payload size in bytes */
    uint8_t buffer[CHUNK_SIZE];       /* Raw binary payload */
    int producer_id;                  /* ID of thread that acquired the chunk */
    bool is_occupied;                 /* Occupancy flag for buffer slot */
};

/* Bounded Circular Buffer in RAM */
typedef struct {
    struct DataChunk slots[BUFFER_CAPACITY];
    int head;                         /* Producer enqueue pointer (IN) */
    int tail;                         /* Consumer dequeue pointer (OUT) */
    int count;                        /* Current occupied slot count */
} BoundedBuffer;

/* Central telemetry, synchronization, and engine state context */
typedef struct {
    /* Circular Bounded Buffer */
    BoundedBuffer buffer;

    /* POSIX Synchronization Primitives */
    pthread_mutex_t buffer_mutex;     /* Mutex guarding circular buffer indices */
    pthread_mutex_t file_io_mutex;    /* Mutex guarding storage lseek/write operations */
    sem_t empty_slots;                /* Semaphore tracking available buffer slots */
    sem_t full_slots;                 /* Semaphore tracking filled buffer slots */

    /* Execution Control & Lifecycle */
    pthread_mutex_t control_mutex;    /* Mutex guarding state transitions & pause/resume */
    pthread_cond_t pause_cond;        /* Condition variable for pause/resume signaling */
    SyncMode sync_mode;               /* SYNCHRONIZED vs UNSYNCHRONIZED */
    EngineState state;                /* Current engine execution state */
    bool is_paused;                   /* Pause flag */
    bool stop_requested;              /* Stop/Cancel flag */

    /* Thread Pool Configuration */
    int num_producers;                /* 1 to 8 producer threads */
    int num_consumers;                /* 1 to 8 consumer threads */
    pthread_t producer_threads[MAX_PRODUCER_THREADS];
    pthread_t consumer_threads[MAX_CONSUMER_THREADS];

    /* Work Partitioning */
    int next_chunk_to_produce;        /* Atomic/mutex index for next available chunk */
    int total_chunks;                 /* Total chunks to download */
    size_t total_file_size;           /* Total size in bytes */
    int chunks_written;               /* Total chunks successfully committed to disk */

    /* Target Paths */
    char target_url[512];             /* Remote URL or loopback endpoint */
    char output_filepath[512];        /* Destination file on disk */
    int output_fd;                    /* Shared file descriptor for consumer writes */

    /* Telemetry Counters */
    uint64_t mutex_contentions;       /* Number of mutex lock collisions recorded */
    uint64_t race_corrupted_blocks;   /* Number of file offset collisions detected */
    uint64_t bytes_downloaded;        /* Total bytes fetched by network producers */
    uint64_t bytes_written;           /* Total bytes written to disk by consumers */
    double transfer_rate_mbps;        /* Moving average transfer speed */
    struct timespec start_time;
    struct timespec end_time;

    /* Block Matrix Map for Cairo Visualization */
    BlockStatus block_statuses[MAX_CHUNKS];
    int block_producer_id[MAX_CHUNKS];

    /* Integrity Verification (OpenSSL SHA-256) */
    char expected_sha256[65];         /* Computed or expected digest */
    char actual_sha256[65];           /* Digest computed from output file */
    bool sha256_verified;

    /* Ring Buffer for Monospace Technical Logs */
    pthread_mutex_t log_mutex;
    char log_messages[LOG_BUFFER_CAPACITY][LOG_LINE_MAX_LEN];
    int log_head;                     /* Next insertion slot in ring */
    int log_count;                    /* Total active log lines (up to capacity) */

    /* Loopback Server Control */
    bool local_server_active;
    int local_server_port;
    char local_benchmark_file[512];

    /* Network Telemetry & Auto-Tune */
    double probe_rtt_ms;              /* Measured RTT in milliseconds during probe */
    bool auto_tune_enabled;           /* Adaptive concurrency tuning flag */
} AppContext;

/* =========================================================================
 * CORE ENGINE API PROTOTYPES
 * ========================================================================= */

void app_context_init(AppContext *ctx);
void app_context_destroy(AppContext *ctx);
void app_context_reset(AppContext *ctx);

/* High-resolution technical logger: formats timestamp HH:MM:SS.mmm and tags */
void app_log(AppContext *ctx, const char *thread_tag, const char *action, const char *fmt, ...);

/* SHA-256 Cryptographic Digest Engine */
int compute_file_sha256(const char *filepath, char *out_hash_hex);

#endif /* COMMON_H */

