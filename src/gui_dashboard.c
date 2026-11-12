#include "gui_dashboard.h"
#include "network_producer.h"
#include "storage_consumer.h"
#include <math.h>

/* =========================================================================
 * HIGH-DENSITY INDUSTRIAL WORKSTATION THEME (MATTE CARBON & OXIDE)
 * Strictly adheres to Anti-AI design guidelines:
 * - No neon glows, no gradients, no emojis.
 * - Hairline 1px borders, 2-3px radius, crisp typography.
 * ========================================================================= */
static const char *WORKSTATION_CSS = 
    "* {\n"
    "    font-family: 'Ubuntu Sans', 'Inter', 'Cantarell', sans-serif;\n"
    "    font-size: 12px;\n"
    "    color: #E5E5E0;\n"
    "}\n"
    "window, .main-window {\n"
    "    background-color: #121211;\n"
    "}\n"
    ".top-toolbar {\n"
    "    background-color: #181816;\n"
    "    border-bottom: 1px solid #282824;\n"
    "    padding: 6px 12px;\n"
    "}\n"
    ".sidebar-panel {\n"
    "    background-color: #161614;\n"
    "    border-right: 1px solid #262622;\n"
    "    padding: 10px 12px;\n"
    "}\n"
    ".workspace-panel {\n"
    "    background-color: #121211;\n"
    "    padding: 10px 14px;\n"
    "}\n"
    ".bottom-panel {\n"
    "    background-color: #161614;\n"
    "    border-top: 1px solid #262622;\n"
    "    padding: 8px 14px;\n"
    "}\n"
    ".telemetry-card {\n"
    "    background-color: #1A1A18;\n"
    "    border: 1px solid #2A2A25;\n"
    "    border-radius: 3px;\n"
    "    padding: 6px 10px;\n"
    "    margin-bottom: 5px;\n"
    "}\n"
    ".section-header {\n"
    "    font-size: 10.5px;\n"
    "    font-weight: 700;\n"
    "    letter-spacing: 0.9px;\n"
    "    color: #7E7E78;\n"
    "    margin-bottom: 4px;\n"
    "}\n"
    ".telemetry-key {\n"
    "    font-size: 10.5px;\n"
    "    font-weight: 600;\n"
    "    color: #85857F;\n"
    "    letter-spacing: 0.4px;\n"
    "}\n"
    ".telemetry-val {\n"
    "    font-family: 'JetBrains Mono', 'Ubuntu Mono', monospace;\n"
    "    font-size: 13px;\n"
    "    font-weight: 600;\n"
    "    color: #E5E5E0;\n"
    "    font-feature-settings: 'tnum';\n"
    "}\n"
    ".telemetry-val-large {\n"
    "    font-family: 'JetBrains Mono', 'Ubuntu Mono', monospace;\n"
    "    font-size: 17px;\n"
    "    font-weight: 700;\n"
    "    color: #E5E5E0;\n"
    "    font-feature-settings: 'tnum';\n"
    "}\n"
    ".telemetry-val-alert {\n"
    "    font-family: 'JetBrains Mono', 'Ubuntu Mono', monospace;\n"
    "    font-size: 14px;\n"
    "    font-weight: 700;\n"
    "    color: #C94A4A;\n"
    "    font-feature-settings: 'tnum';\n"
    "}\n"
    ".telemetry-val-success {\n"
    "    font-family: 'JetBrains Mono', 'Ubuntu Mono', monospace;\n"
    "    font-size: 13px;\n"
    "    font-weight: 600;\n"
    "    color: #4E9A67;\n"
    "}\n"
    "button.btn-primary {\n"
    "    background-color: #E5E5E0;\n"
    "    color: #121211;\n"
    "    font-weight: 700;\n"
    "    letter-spacing: 0.5px;\n"
    "    border: 1px solid #E5E5E0;\n"
    "    border-radius: 3px;\n"
    "    padding: 5px 16px;\n"
    "}\n"
    "button.btn-primary:hover {\n"
    "    background-color: #FFFFFF;\n"
    "}\n"
    "button.btn-secondary {\n"
    "    background-color: #1F1F1D;\n"
    "    color: #E5E5E0;\n"
    "    font-weight: 600;\n"
    "    border: 1px solid #30302B;\n"
    "    border-radius: 3px;\n"
    "    padding: 5px 12px;\n"
    "}\n"
    "button.btn-secondary:hover {\n"
    "    background-color: #282824;\n"
    "}\n"
    "button.btn-steel {\n"
    "    background-color: #24415C;\n"
    "    color: #E5E5E0;\n"
    "    font-weight: 600;\n"
    "    border: 1px solid #355E85;\n"
    "    border-radius: 3px;\n"
    "    padding: 5px 12px;\n"
    "}\n"
    "button.btn-steel:hover {\n"
    "    background-color: #2F5375;\n"
    "}\n"
    "button.btn-active {\n"
    "    background-color: #2B5B84;\n"
    "    color: #FFFFFF;\n"
    "    border: 1px solid #4A88C7;\n"
    "    font-weight: 700;\n"
    "}\n"
    "button.btn-auto-on {\n"
    "    background-color: #162E44;\n"
    "    color: #5AA9E6;\n"
    "    border: 1px solid #2B5B84;\n"
    "    font-weight: 700;\n"
    "    font-size: 11px;\n"
    "    font-family: 'JetBrains Mono', monospace;\n"
    "    border-radius: 3px;\n"
    "    padding: 4px 10px;\n"
    "}\n"
    "button.btn-auto-off {\n"
    "    background-color: #1F1F1D;\n"
    "    color: #85857F;\n"
    "    border: 1px solid #30302B;\n"
    "    font-weight: 600;\n"
    "    font-size: 11px;\n"
    "    font-family: 'JetBrains Mono', monospace;\n"
    "    border-radius: 3px;\n"
    "    padding: 4px 10px;\n"
    "}\n"
    ".badge-dataset {\n"
    "    background-color: #121E2B;\n"
    "    color: #4A88C7;\n"
    "    border: 1px solid #294D70;\n"
    "    border-radius: 2px;\n"
    "    padding: 3px 8px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11px;\n"
    "    font-family: 'JetBrains Mono', monospace;\n"
    "}\n"
    ".badge-dataset-net {\n"
    "    background-color: #281F12;\n"
    "    color: #D4973B;\n"
    "    border: 1px solid #6E4E20;\n"
    "    border-radius: 2px;\n"
    "    padding: 3px 8px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11px;\n"
    "    font-family: 'JetBrains Mono', monospace;\n"
    "}\n"
    "entry {\n"
    "    background-color: #0E0E0D;\n"
    "    color: #E5E5E0;\n"
    "    border: 1px solid #282824;\n"
    "    border-radius: 3px;\n"
    "    padding: 4px 8px;\n"
    "    font-family: 'JetBrains Mono', monospace;\n"
    "}\n"
    "textview text {\n"
    "    background-color: #0E0E0D;\n"
    "    color: #E5E5E0;\n"
    "    font-family: 'JetBrains Mono', 'Ubuntu Mono', monospace;\n"
    "    font-size: 11px;\n"
    "}\n"
    "progressbar trough {\n"
    "    background-color: #0E0E0D;\n"
    "    border: 1px solid #282824;\n"
    "    border-radius: 2px;\n"
    "    min-height: 8px;\n"
    "}\n"
    "progressbar progress {\n"
    "    background-color: #4E9A67;\n"
    "    border-radius: 0px;\n"
    "}\n"
    ".badge-idle {\n"
    "    background-color: #1A1A18;\n"
    "    color: #85857F;\n"
    "    border: 1px solid #2D2D28;\n"
    "    border-radius: 2px;\n"
    "    padding: 3px 10px;\n"
    "    font-weight: 600;\n"
    "    font-size: 11px;\n"
    "    font-family: 'JetBrains Mono', monospace;\n"
    "}\n"
    ".badge-running {\n"
    "    background-color: #162432;\n"
    "    color: #4A88C7;\n"
    "    border: 1px solid #294D70;\n"
    "    border-radius: 2px;\n"
    "    padding: 3px 10px;\n"
    "    font-weight: 600;\n"
    "    font-size: 11px;\n"
    "    font-family: 'JetBrains Mono', monospace;\n"
    "}\n"
    ".badge-verified {\n"
    "    background-color: #15251A;\n"
    "    color: #4E9A67;\n"
    "    border: 1px solid #264A30;\n"
    "    border-radius: 2px;\n"
    "    padding: 3px 10px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11px;\n"
    "    font-family: 'JetBrains Mono', monospace;\n"
    "}\n"
    ".badge-corrupted {\n"
    "    background-color: #2D1414;\n"
    "    color: #C94A4A;\n"
    "    border: 1px solid #5A2424;\n"
    "    border-radius: 2px;\n"
    "    padding: 3px 10px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11px;\n"
    "    font-family: 'JetBrains Mono', monospace;\n"
    "}\n"
    ".mode-synced {\n"
    "    color: #4E9A67;\n"
    "    font-weight: 700;\n"
    "    font-size: 11px;\n"
    "}\n"
    ".mode-unsynced {\n"
    "    color: #C94A4A;\n"
    "    font-weight: 700;\n"
    "    font-size: 11px;\n"
    "}\n"
    "separator {\n"
    "    background-color: #262622;\n"
    "    min-width: 1px;\n"
    "    min-height: 1px;\n"
    "}\n"
    "/* Left Navigation Sidebar (Industrial White/Slate) */\n"
    ".nav-sidebar {\n"
    "    background-color: #0E0F12;\n"
    "    border-right: 1px solid #22252E;\n"
    "    padding: 12px 10px;\n"
    "}\n"
    ".nav-brand-title {\n"
    "    font-size: 13px;\n"
    "    font-weight: 800;\n"
    "    letter-spacing: 0.8px;\n"
    "    color: #FFFFFF;\n"
    "}\n"
    ".nav-brand-subtitle {\n"
    "    font-size: 10px;\n"
    "    font-weight: 600;\n"
    "    color: #717A8C;\n"
    "    letter-spacing: 0.5px;\n"
    "    margin-bottom: 8px;\n"
    "}\n"
    ".nav-section-label {\n"
    "    font-size: 9.5px;\n"
    "    font-weight: 700;\n"
    "    color: #64748B;\n"
    "    letter-spacing: 0.8px;\n"
    "    margin-top: 8px;\n"
    "    margin-bottom: 6px;\n"
    "    padding-left: 4px;\n"
    "}\n"
    "button.nav-item {\n"
    "    background-color: transparent;\n"
    "    color: #94A3B8;\n"
    "    border: 1px solid transparent;\n"
    "    border-radius: 4px;\n"
    "    padding: 8px 10px;\n"
    "    font-size: 12px;\n"
    "    font-weight: 600;\n"
    "}\n"
    "button.nav-item:hover {\n"
    "    background-color: #1A1D24;\n"
    "    color: #FFFFFF;\n"
    "    border: 1px solid #282C37;\n"
    "}\n"
    "button.nav-item-active {\n"
    "    background-color: #1F2430;\n"
    "    color: #FFFFFF;\n"
    "    border: 1px solid #363E52;\n"
    "    border-left: 3px solid #FFFFFF;\n"
    "    font-weight: 700;\n"
    "}\n"
    ".nav-badge-count {\n"
    "    background-color: #1E232F;\n"
    "    color: #93C5FD;\n"
    "    border: 1px solid #2B3448;\n"
    "    border-radius: 10px;\n"
    "    padding: 1px 7px;\n"
    "    font-size: 10px;\n"
    "    font-family: 'JetBrains Mono', monospace;\n"
    "    font-weight: 700;\n"
    "}\n"
    ".nav-footer-box {\n"
    "    background-color: #13151A;\n"
    "    border: 1px solid #20242D;\n"
    "    border-radius: 3px;\n"
    "    padding: 8px;\n"
    "    margin-top: 6px;\n"
    "}\n"
    ".nav-footer-text {\n"
    "    font-size: 10px;\n"
    "    color: #64748B;\n"
    "    font-family: 'JetBrains Mono', monospace;\n"
    "}\n"
    "/* Page 2: Download History Panel */\n"
    ".history-page {\n"
    "    background-color: #121316;\n"
    "    padding: 16px 20px;\n"
    "}\n"
    ".history-header-title {\n"
    "    font-size: 16px;\n"
    "    font-weight: 800;\n"
    "    letter-spacing: 0.5px;\n"
    "    color: #FFFFFF;\n"
    "}\n"
    ".history-header-desc {\n"
    "    font-size: 11.5px;\n"
    "    color: #858997;\n"
    "    margin-bottom: 12px;\n"
    "}\n"
    ".history-card-metric {\n"
    "    background-color: #171920;\n"
    "    border: 1px solid #262A36;\n"
    "    border-radius: 4px;\n"
    "    padding: 10px 14px;\n"
    "}\n"
    ".history-metric-title {\n"
    "    font-size: 10px;\n"
    "    font-weight: 700;\n"
    "    color: #7E8597;\n"
    "    letter-spacing: 0.6px;\n"
    "}\n"
    ".history-metric-val {\n"
    "    font-family: 'JetBrains Mono', monospace;\n"
    "    font-size: 20px;\n"
    "    font-weight: 700;\n"
    "    color: #FFFFFF;\n"
    "}\n"
    ".history-metric-val-ok {\n"
    "    font-family: 'JetBrains Mono', monospace;\n"
    "    font-size: 20px;\n"
    "    font-weight: 700;\n"
    "    color: #4E9A67;\n"
    "}\n"
    ".history-metric-val-err {\n"
    "    font-family: 'JetBrains Mono', monospace;\n"
    "    font-size: 20px;\n"
    "    font-weight: 700;\n"
    "    color: #C94A4A;\n"
    "}\n"
    "treeview.history-tree {\n"
    "    background-color: #0E0F12;\n"
    "    color: #D1D5DB;\n"
    "    border: 1px solid #22252E;\n"
    "    font-size: 11.5px;\n"
    "}\n"
    "treeview.history-tree:selected {\n"
    "    background-color: #242936;\n"
    "    color: #FFFFFF;\n"
    "}\n"
    "treeview.history-tree header button {\n"
    "    background-color: #171920;\n"
    "    color: #9CA3AF;\n"
    "    font-weight: 700;\n"
    "    font-size: 10.5px;\n"
    "    border: 1px solid #22252E;\n"
    "    padding: 6px;\n"
    "}\n";

/* =========================================================================
 * ASYNC IDLE HANDLER FOR HISTORY UPDATES
 * ========================================================================= */
typedef struct {
    GuiDashboard *gui;
    DownloadHistoryRecord rec;
} HistoryIdleData;

static gboolean on_history_idle_cb(gpointer user_data) {
    HistoryIdleData *data = (HistoryIdleData *)user_data;
    if (data && data->gui) {
        gui_add_history_record(data->gui, &data->rec);
    }
    free(data);
    return G_SOURCE_REMOVE;
}

/* =========================================================================
 * BACKGROUND ENGINE COORDINATOR THREAD
 * ========================================================================= */
static void* engine_coordinator_thread(void *arg) {
    GuiDashboard *gui = (GuiDashboard *)arg;
    AppContext *ctx = gui->ctx;

    app_log(ctx, "ENGINE_MGR", "START", "Starting multi-threaded transfer pipeline...");

    mkdir("test_data", 0755);
    const char *out_file = (ctx->sync_mode == SYNC_MODE_SYNCHRONIZED) 
                           ? "test_data/output_sync.bin" 
                           : "test_data/output_unsync.bin";

    if (prepare_output_file(ctx, out_file) != 0) {
        app_log(ctx, "ENGINE_MGR", "ERROR", "Could not initialize target storage container");
        ctx->state = STATE_ERROR;
        gui->worker_running = false;

        /* Record failed download attempt in History */
        HistoryIdleData *hid = (HistoryIdleData *)malloc(sizeof(HistoryIdleData));
        if (hid) {
            hid->gui = gui;
            time_t now = time(NULL);
            struct tm tm_buf;
            localtime_r(&now, &tm_buf);
            strftime(hid->rec.timestamp, sizeof(hid->rec.timestamp), "%H:%M:%S", &tm_buf);
            snprintf(hid->rec.filename, sizeof(hid->rec.filename), "DOWNLOAD_FAILED");
            snprintf(hid->rec.filepath, sizeof(hid->rec.filepath), "-");
            snprintf(hid->rec.target_url, sizeof(hid->rec.target_url), "%s", ctx->target_url);
            hid->rec.file_size = 0;
            hid->rec.duration_sec = 0.0;
            hid->rec.speed_mbps = 0.0;
            hid->rec.sync_mode = ctx->sync_mode;
            hid->rec.is_verified = false;
            hid->rec.race_errors = 1;
            snprintf(hid->rec.sha256, sizeof(hid->rec.sha256), "N/A - NETWORK ERROR");
            g_idle_add(on_history_idle_cb, hid);
        }
        return NULL;
    }

    clock_gettime(CLOCK_MONOTONIC, &ctx->start_time);
    ctx->state = STATE_RUNNING;

    /* Launch Storage Consumers first */
    start_consumer_pool(ctx);

    /* Launch Network Producers to fetch chunks */
    start_producer_pool(ctx);

    /* Wait for Producers to finish downloading */
    wait_producer_pool(ctx);

    /* Wait for Consumers to finish writing */
    wait_consumer_pool(ctx);

    clock_gettime(CLOCK_MONOTONIC, &ctx->end_time);

    /* Post-transfer cryptographic audit */
    verify_file_integrity(ctx);

    /* For external URLs, also expose original filename in test_data/ */
    char final_filepath[512];
    snprintf(final_filepath, sizeof(final_filepath), "%s", out_file);

    const char *last_slash = strrchr(ctx->target_url, '/');
    if (last_slash && strlen(last_slash + 1) > 0 && strncmp(ctx->target_url, "http://127.0.0.1", 16) != 0) {
        char orig_fname[256];
        strncpy(orig_fname, last_slash + 1, sizeof(orig_fname) - 1);
        orig_fname[sizeof(orig_fname) - 1] = '\0';
        char *q = strchr(orig_fname, '?');
        if (q) *q = '\0';
        if (strlen(orig_fname) > 0 && strcmp(orig_fname, "output_sync.bin") != 0 && strcmp(orig_fname, "output_unsync.bin") != 0) {
            char link_dest[512];
            snprintf(link_dest, sizeof(link_dest), "test_data/%s", orig_fname);
            unlink(link_dest);
            if (link(out_file, link_dest) == 0) {
                app_log(ctx, "ENGINE_MGR", "SAVED", "Saved with original name: %s", link_dest);
                snprintf(final_filepath, sizeof(final_filepath), "%s", link_dest);
            }
        }
    }

    ctx->state = STATE_COMPLETED;
    gui->worker_running = false;

    app_log(ctx, "ENGINE_MGR", "COMPLETE", "Transfer pipeline concluded. File stored at: %s", out_file);

    /* Record completed transfer in Session History */
    HistoryIdleData *hid = (HistoryIdleData *)malloc(sizeof(HistoryIdleData));
    if (hid) {
        hid->gui = gui;
        time_t now = time(NULL);
        struct tm tm_buf;
        localtime_r(&now, &tm_buf);
        strftime(hid->rec.timestamp, sizeof(hid->rec.timestamp), "%H:%M:%S", &tm_buf);

        const char *slash = strrchr(final_filepath, '/');
        const char *fname = slash ? slash + 1 : final_filepath;
        snprintf(hid->rec.filename, sizeof(hid->rec.filename), "%.255s", fname);
        snprintf(hid->rec.filepath, sizeof(hid->rec.filepath), "%.511s", final_filepath);
        snprintf(hid->rec.target_url, sizeof(hid->rec.target_url), "%.511s", ctx->target_url);
        hid->rec.file_size = ctx->total_file_size;

        double elapsed = (ctx->end_time.tv_sec - ctx->start_time.tv_sec) +
                         (ctx->end_time.tv_nsec - ctx->start_time.tv_nsec) / 1e9;
        if (elapsed <= 0.001) elapsed = 0.001;
        hid->rec.duration_sec = elapsed;
        hid->rec.speed_mbps = (ctx->total_file_size / (1024.0 * 1024.0)) / elapsed;
        hid->rec.sync_mode = ctx->sync_mode;
        hid->rec.is_verified = (ctx->race_corrupted_blocks == 0 && ctx->sha256_verified);
        hid->rec.race_errors = ctx->race_corrupted_blocks;
        snprintf(hid->rec.sha256, sizeof(hid->rec.sha256), "%s", ctx->actual_sha256);

        g_idle_add(on_history_idle_cb, hid);
    }

    return NULL;
}

/* =========================================================================
 * CAIRO 2D DRAWING: RAM BOUNDED BUFFER INSPECTOR (TOP CANVAS)
 * ========================================================================= */
gboolean on_draw_ram_buffer(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    GuiDashboard *gui = (GuiDashboard *)user_data;
    AppContext *ctx = gui->ctx;

    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    /* Dark Deep Inset Background */
    cairo_set_source_rgb(cr, 0.055, 0.055, 0.051); /* #0E0E0D */
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    /* Technical Border */
    cairo_set_source_rgb(cr, 0.150, 0.150, 0.138); /* #262623 */
    cairo_set_line_width(cr, 1.0);
    cairo_rectangle(cr, 0.5, 0.5, width - 1, height - 1);
    cairo_stroke(cr);

    /* Layout geometry: 32 slots horizontally */
    double margin_x = 20.0;
    double slot_top_y = 32.0;
    double slot_height = 42.0;
    double available_w = width - (2 * margin_x);
    double gap = 3.0;
    double slot_w = (available_w - ((BUFFER_CAPACITY - 1) * gap)) / (double)BUFFER_CAPACITY;

    pthread_mutex_lock(&ctx->buffer_mutex);
    int head_idx = ctx->buffer.head;
    int tail_idx = ctx->buffer.tail;
    int count = ctx->buffer.count;
    pthread_mutex_unlock(&ctx->buffer_mutex);

    cairo_select_font_face(cr, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

    for (int i = 0; i < BUFFER_CAPACITY; i++) {
        double x = margin_x + i * (slot_w + gap);
        bool is_occupied = ctx->buffer.slots[i].is_occupied;

        /* Micro memory address label above (0x00, 0x01 ... 0x1F) */
        cairo_set_source_rgb(cr, 0.40, 0.40, 0.38);
        cairo_set_font_size(cr, 7.5);
        char hex_addr[8];
        snprintf(hex_addr, sizeof(hex_addr), "%02X", i);
        cairo_text_extents_t aext;
        cairo_text_extents(cr, hex_addr, &aext);
        cairo_move_to(cr, x + (slot_w - aext.width) / 2.0 - aext.x_bearing, slot_top_y - 14.0);
        cairo_show_text(cr, hex_addr);

        /* Slot Body */
        if (is_occupied) {
            /* Warm Ochre: Queued in RAM */
            cairo_set_source_rgb(cr, 0.831, 0.592, 0.231); /* #D4973B */
            cairo_rectangle(cr, x, slot_top_y, slot_w, slot_height);
            cairo_fill(cr);

            /* Chunk ID inside slot */
            cairo_set_source_rgb(cr, 0.078, 0.078, 0.075); /* #141413 */
            cairo_set_font_size(cr, 9.0);
            char cid_str[16];
            snprintf(cid_str, sizeof(cid_str), "%d", ctx->buffer.slots[i].chunk_id);
            cairo_text_extents_t ext;
            cairo_text_extents(cr, cid_str, &ext);
            cairo_move_to(cr, x + (slot_w - ext.width) / 2.0 - ext.x_bearing,
                              slot_top_y + (slot_height / 2.0) - 2.0);
            cairo_show_text(cr, cid_str);

            /* Producer ID badge (P1..P4) */
            int pid = ctx->buffer.slots[i].producer_id;
            if (pid > 0) {
                cairo_set_source_rgb(cr, 0.20, 0.20, 0.18);
                cairo_set_font_size(cr, 7.5);
                char pid_str[16];
                snprintf(pid_str, sizeof(pid_str), "P%d", pid);
                cairo_text_extents_t pext;
                cairo_text_extents(cr, pid_str, &pext);
                cairo_move_to(cr, x + (slot_w - pext.width) / 2.0 - pext.x_bearing,
                                  slot_top_y + slot_height - 6.0);
                cairo_show_text(cr, pid_str);
            }
        } else {
            /* Empty slot */
            cairo_set_source_rgb(cr, 0.090, 0.090, 0.086); /* Inset dark */
            cairo_rectangle(cr, x, slot_top_y, slot_w, slot_height);
            cairo_fill(cr);
        }

        /* Slot Hairline Border */
        cairo_set_source_rgb(cr, 0.160, 0.160, 0.145);
        cairo_set_line_width(cr, 1.0);
        cairo_rectangle(cr, x + 0.5, slot_top_y + 0.5, slot_w - 1, slot_height - 1);
        cairo_stroke(cr);

        /* Decimal slot index below */
        cairo_set_source_rgb(cr, 0.50, 0.50, 0.48);
        cairo_set_font_size(cr, 8.0);
        char idx_str[8];
        snprintf(idx_str, sizeof(idx_str), "%02d", i);
        cairo_text_extents_t iext;
        cairo_text_extents(cr, idx_str, &iext);
        cairo_move_to(cr, x + (slot_w - iext.width) / 2.0 - iext.x_bearing, slot_top_y + slot_height + 11.0);
        cairo_show_text(cr, idx_str);

        /* =============================================================
         * HEAD (IN) POINTER: STEEL BLUE ARROW ABOVE
         * ============================================================= */
        if (i == head_idx) {
            double center_x = x + slot_w / 2.0;
            cairo_set_source_rgb(cr, 0.290, 0.533, 0.780); /* #4A88C7 */

            cairo_move_to(cr, center_x, slot_top_y - 2.0);
            cairo_line_to(cr, center_x - 4.5, slot_top_y - 7.5);
            cairo_line_to(cr, center_x + 4.5, slot_top_y - 7.5);
            cairo_close_path(cr);
            cairo_fill(cr);

            cairo_set_font_size(cr, 8.5);
            const char *head_lbl = "HEAD";
            cairo_text_extents_t hext;
            cairo_text_extents(cr, head_lbl, &hext);
            cairo_move_to(cr, center_x - hext.width / 2.0 - hext.x_bearing, slot_top_y - 8.5);
            cairo_show_text(cr, head_lbl);
        }

        /* =============================================================
         * TAIL (OUT) POINTER: SAGE GREEN ARROW BELOW
         * ============================================================= */
        if (i == tail_idx) {
            double center_x = x + slot_w / 2.0;
            double arrow_base_y = slot_top_y + slot_height + 14.0;
            cairo_set_source_rgb(cr, 0.306, 0.604, 0.404); /* #4E9A67 */

            cairo_move_to(cr, center_x, arrow_base_y);
            cairo_line_to(cr, center_x - 4.5, arrow_base_y + 5.5);
            cairo_line_to(cr, center_x + 4.5, arrow_base_y + 5.5);
            cairo_close_path(cr);
            cairo_fill(cr);

            cairo_set_font_size(cr, 8.5);
            const char *tail_lbl = "TAIL";
            cairo_text_extents_t text;
            cairo_text_extents(cr, tail_lbl, &text);
            cairo_move_to(cr, center_x - text.width / 2.0 - text.x_bearing, arrow_base_y + 15.0);
            cairo_show_text(cr, tail_lbl);
        }
    }

    /* Top Left Status Text */
    cairo_set_source_rgb(cr, 0.70, 0.70, 0.68);
    cairo_set_font_size(cr, 9.5);
    cairo_move_to(cr, margin_x, 14.0);
    cairo_show_text(cr, "CIRCULAR BUFFER MAP // 32 SLOTS (64KB EACH)");

    /* Top Right Occupancy Gauge */
    double pct = (count / (double)BUFFER_CAPACITY) * 100.0;
    char occ_str[64];
    snprintf(occ_str, sizeof(occ_str), "OCCUPANCY: %02d/32 (%.1f%%)", count, pct);
    cairo_text_extents_t oext;
    cairo_text_extents(cr, occ_str, &oext);
    cairo_set_source_rgb(cr, 0.831, 0.592, 0.231);
    cairo_move_to(cr, width - margin_x - oext.width, 14.0);
    cairo_show_text(cr, occ_str);

    return FALSE;
}

/* =========================================================================
 * CAIRO 2D DRAWING: DISK BLOCK ALLOCATION MATRIX (BOTTOM CANVAS)
 * ========================================================================= */
gboolean on_draw_block_matrix(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    GuiDashboard *gui = (GuiDashboard *)user_data;
    AppContext *ctx = gui->ctx;

    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    /* Background */
    cairo_set_source_rgb(cr, 0.055, 0.055, 0.051); /* #0E0E0D */
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    /* Border */
    cairo_set_source_rgb(cr, 0.150, 0.150, 0.138); /* #262623 */
    cairo_set_line_width(cr, 1.0);
    cairo_rectangle(cr, 0.5, 0.5, width - 1, height - 1);
    cairo_stroke(cr);

    /* Matrix Layout: 32 columns x 8 rows = 256 blocks */
    int cols = 32;
    int rows = 8;
    double margin_x = 16.0;
    double margin_y = 12.0;
    double gap = 2.0;

    double avail_w = width - (2 * margin_x);
    double cell_w = (avail_w - (cols - 1) * gap) / cols;
    double avail_h = height - (2 * margin_y) - 24.0; /* Reserve space for legend */
    double cell_h = (avail_h - (rows - 1) * gap) / rows;

    int total_to_render = (ctx->total_chunks > 0 && ctx->total_chunks <= MAX_CHUNKS) ? ctx->total_chunks : 256;

    int count_unassigned = 0;
    int count_downloading = 0;
    int count_queued = 0;
    int count_written = 0;
    int count_corrupted = 0;

    for (int i = 0; i < total_to_render; i++) {
        int r = i / cols;
        int c = i % cols;
        if (r >= rows) break;

        double x = margin_x + c * (cell_w + gap);
        double y = margin_y + r * (cell_h + gap);

        BlockStatus st = ctx->block_statuses[i];

        switch (st) {
            case BLOCK_UNASSIGNED:
                cairo_set_source_rgb(cr, 0.137, 0.137, 0.129); /* #232321 */
                count_unassigned++;
                break;
            case BLOCK_DOWNLOADING:
                cairo_set_source_rgb(cr, 0.290, 0.533, 0.780); /* #4A88C7 Steel Blue */
                count_downloading++;
                break;
            case BLOCK_QUEUED:
                cairo_set_source_rgb(cr, 0.831, 0.592, 0.231); /* #D4973B Warm Ochre */
                count_queued++;
                break;
            case BLOCK_WRITTEN:
                cairo_set_source_rgb(cr, 0.306, 0.604, 0.404); /* #4E9A67 Sage Green */
                count_written++;
                break;
            case BLOCK_CORRUPTED:
                cairo_set_source_rgb(cr, 0.788, 0.290, 0.290); /* #C94A4A Brick Crimson */
                count_corrupted++;
                break;
        }

        cairo_rectangle(cr, x, y, cell_w, cell_h);
        cairo_fill(cr);

        /* If corrupted, draw bold white cross indicator */
        if (st == BLOCK_CORRUPTED) {
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
            cairo_set_line_width(cr, 1.5);
            cairo_move_to(cr, x + 2.5, y + 2.5);
            cairo_line_to(cr, x + cell_w - 2.5, y + cell_h - 2.5);
            cairo_move_to(cr, x + cell_w - 2.5, y + 2.5);
            cairo_line_to(cr, x + 2.5, y + cell_h - 2.5);
            cairo_stroke(cr);
        }

        /* 1px Cell Border */
        cairo_set_source_rgb(cr, 0.090, 0.090, 0.085);
        cairo_set_line_width(cr, 1.0);
        cairo_rectangle(cr, x + 0.5, y + 0.5, cell_w - 1, cell_h - 1);
        cairo_stroke(cr);
    }

    /* =============================================================
     * DYNAMIC TECHNICAL LEGEND WITH LIVE BLOCK COUNTERS
     * ============================================================= */
    double leg_y = height - 12.0;
    double leg_x = margin_x;

    typedef struct {
        const char *label;
        int count;
        double r, g, b;
    } LegendEntry;

    LegendEntry legend_entries[] = {
        {"UNASSIGNED", count_unassigned, 0.137, 0.137, 0.129},
        {"PRODUCER ACTIVE", count_downloading, 0.290, 0.533, 0.780},
        {"QUEUED (RAM)", count_queued, 0.831, 0.592, 0.231},
        {"COMMITTED (SAFE)", count_written, 0.306, 0.604, 0.404},
        {"RACE CORRUPTED", count_corrupted, 0.788, 0.290, 0.290}
    };

    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 9.0);

    for (int k = 0; k < 5; k++) {
        /* 8x8px square badge */
        cairo_set_source_rgb(cr, legend_entries[k].r, legend_entries[k].g, legend_entries[k].b);
        cairo_rectangle(cr, leg_x, leg_y - 7.5, 8, 8);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 0.55, 0.55, 0.52);
        char str_item[64];
        snprintf(str_item, sizeof(str_item), "%s: %d", legend_entries[k].label, legend_entries[k].count);
        cairo_move_to(cr, leg_x + 12, leg_y);
        cairo_show_text(cr, str_item);

        cairo_text_extents_t lext;
        cairo_text_extents(cr, str_item, &lext);
        leg_x += lext.width + 24.0;
    }

    return FALSE;
}

/* =========================================================================
 * CAIRO 2D DRAWING: REAL-TIME THROUGHPUT SPARKLINE GRAPH (SIDEBAR)
 * ========================================================================= */
gboolean on_draw_sidebar_sparkline(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    GuiDashboard *gui = (GuiDashboard *)user_data;

    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    /* Dark Background */
    cairo_set_source_rgb(cr, 0.055, 0.055, 0.051); /* #0E0E0D */
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    /* Border */
    cairo_set_source_rgb(cr, 0.150, 0.150, 0.138);
    cairo_set_line_width(cr, 1.0);
    cairo_rectangle(cr, 0.5, 0.5, width - 1, height - 1);
    cairo_stroke(cr);

    /* Reference Grid Lines (0, 150, 300 MB/s) */
    cairo_set_source_rgb(cr, 0.110, 0.110, 0.100);
    cairo_set_line_width(cr, 0.8);
    for (int g = 1; g <= 2; g++) {
        double gy = height - (g * (height / 3.0));
        cairo_move_to(cr, 4, gy);
        cairo_line_to(cr, width - 4, gy);
        cairo_stroke(cr);
    }

    double max_scale = (gui->max_speed_recorded > 10.0) ? (gui->max_speed_recorded * 1.15) : 100.0;

    /* Plot Curve */
    cairo_set_line_width(cr, 1.5);
    cairo_set_source_rgb(cr, 0.290, 0.533, 0.780); /* Steel Blue */

    double step_x = (double)(width - 8) / (double)(SPARKLINE_POINTS - 1);

    for (int p = 0; p < SPARKLINE_POINTS; p++) {
        int idx = (gui->speed_history_idx + p) % SPARKLINE_POINTS;
        double val = gui->speed_history[idx];
        double py = height - 4.0 - ((val / max_scale) * (height - 12.0));
        double px = 4.0 + p * step_x;

        if (p == 0) cairo_move_to(cr, px, py);
        else cairo_line_to(cr, px, py);
    }
    cairo_stroke_preserve(cr);

    /* Gradient Shading Underneath Curve */
    cairo_line_to(cr, width - 4.0, height - 2.0);
    cairo_line_to(cr, 4.0, height - 2.0);
    cairo_close_path(cr);
    cairo_set_source_rgba(cr, 0.290, 0.533, 0.780, 0.15);
    cairo_fill(cr);

    /* Max watermark text */
    cairo_select_font_face(cr, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 8.5);
    cairo_set_source_rgb(cr, 0.45, 0.45, 0.43);
    char max_lbl[32];
    snprintf(max_lbl, sizeof(max_lbl), "PEAK: %.1f MB/s", gui->max_speed_recorded);
    cairo_move_to(cr, 8, 12);
    cairo_show_text(cr, max_lbl);

    return FALSE;
}

/* =========================================================================
 * COLORIZED LOG PARSER & FORMATTER (IDE-GRADE SYNTAX HIGHLIGHTING)
 * ========================================================================= */
static void append_colorized_log_line(GuiDashboard *gui, const char *raw_line) {
    GtkTextIter end_iter;
    gtk_text_buffer_get_end_iter(gui->text_buffer_logs, &end_iter);

    /* Expected format: "14:22:05.104 | P_THREAD_02 | ENQUEUE  | details..." */
    char buf[256];
    strncpy(buf, raw_line, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    char *saveptr = NULL;
    char *timestamp = strtok_r(buf, "|", &saveptr);
    char *thread_tag = strtok_r(NULL, "|", &saveptr);
    char *action = strtok_r(NULL, "|", &saveptr);
    char *detail = strtok_r(NULL, "\n", &saveptr);

    if (timestamp && thread_tag && action && detail) {
        /* Trim trailing/leading whitespace */
        gtk_text_buffer_insert_with_tags(gui->text_buffer_logs, &end_iter, timestamp, -1, gui->tag_timestamp, NULL);
        gtk_text_buffer_insert(gui->text_buffer_logs, &end_iter, "|", -1);

        /* Tag Thread type */
        if (strstr(thread_tag, "P_THREAD")) {
            gtk_text_buffer_insert_with_tags(gui->text_buffer_logs, &end_iter, thread_tag, -1, gui->tag_producer, NULL);
        } else if (strstr(thread_tag, "C_THREAD")) {
            gtk_text_buffer_insert_with_tags(gui->text_buffer_logs, &end_iter, thread_tag, -1, gui->tag_consumer, NULL);
        } else {
            gtk_text_buffer_insert_with_tags(gui->text_buffer_logs, &end_iter, thread_tag, -1, gui->tag_action_sys, NULL);
        }
        gtk_text_buffer_insert(gui->text_buffer_logs, &end_iter, "|", -1);

        /* Tag Action type */
        if (strstr(action, "RACE_ERR")) {
            gtk_text_buffer_insert_with_tags(gui->text_buffer_logs, &end_iter, action, -1, gui->tag_action_race, NULL);
        } else if (strstr(action, "COMMIT")) {
            gtk_text_buffer_insert_with_tags(gui->text_buffer_logs, &end_iter, action, -1, gui->tag_action_commit, NULL);
        } else if (strstr(action, "ENQUEUE") || strstr(action, "DEQUEUE")) {
            gtk_text_buffer_insert_with_tags(gui->text_buffer_logs, &end_iter, action, -1, gui->tag_action_enqueue, NULL);
        } else {
            gtk_text_buffer_insert_with_tags(gui->text_buffer_logs, &end_iter, action, -1, gui->tag_action_sys, NULL);
        }
        gtk_text_buffer_insert(gui->text_buffer_logs, &end_iter, "|", -1);

        /* Detail string */
        gtk_text_buffer_insert(gui->text_buffer_logs, &end_iter, detail, -1);
        gtk_text_buffer_insert(gui->text_buffer_logs, &end_iter, "\n", -1);
    } else {
        /* Fallback: insert unformatted line */
        gtk_text_buffer_insert(gui->text_buffer_logs, &end_iter, raw_line, -1);
        gtk_text_buffer_insert(gui->text_buffer_logs, &end_iter, "\n", -1);
    }
}

/* =========================================================================
 * PERIODIC TELEMETRY TICK (30 FPS / ~33ms)
 * ========================================================================= */
gboolean on_gui_telemetry_tick(gpointer user_data) {
    GuiDashboard *gui = (GuiDashboard *)user_data;
    AppContext *ctx = gui->ctx;

    /* Calculate Moving Speed */
    double elapsed = 0.001;
    if (ctx->state == STATE_RUNNING) {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        elapsed = (now.tv_sec - ctx->start_time.tv_sec) + 
                  (now.tv_nsec - ctx->start_time.tv_nsec) / 1e9;
    } else if (ctx->state == STATE_COMPLETED) {
        elapsed = (ctx->end_time.tv_sec - ctx->start_time.tv_sec) + 
                  (ctx->end_time.tv_nsec - ctx->start_time.tv_nsec) / 1e9;
    }
    if (elapsed <= 0.0) elapsed = 0.001;

    double mb_written = ctx->bytes_written / (1024.0 * 1024.0);
    double mb_downloaded = ctx->bytes_downloaded / (1024.0 * 1024.0);
    double total_mb = ctx->total_file_size / (1024.0 * 1024.0);
    double speed_mbps = (ctx->state == STATE_RUNNING) ? (mb_written / elapsed) : 0.0;
    if (ctx->state == STATE_COMPLETED) speed_mbps = mb_written / elapsed;

    /* Push speed to history buffer for Sparkline */
    gui->speed_history[gui->speed_history_idx] = speed_mbps;
    gui->speed_history_idx = (gui->speed_history_idx + 1) % SPARKLINE_POINTS;
    if (speed_mbps > gui->max_speed_recorded) {
        gui->max_speed_recorded = speed_mbps;
    }

    char str_buf[128];

    /* Engine State Label */
    const char *state_str = "STANDBY [IDLE]";
    if (ctx->state == STATE_RUNNING) state_str = ctx->is_paused ? "PAUSED" : "ACTIVE [TRANSFERRING]";
    else if (ctx->state == STATE_COMPLETED) state_str = "FINISHED [COMPLETED]";
    else if (ctx->state == STATE_ERROR) state_str = "FAULT [ERROR]";
    gtk_label_set_text(GTK_LABEL(gui->lbl_engine_state), state_str);

    /* Transfer Rate */
    snprintf(str_buf, sizeof(str_buf), "%6.2f MB/s", speed_mbps);
    gtk_label_set_text(GTK_LABEL(gui->lbl_rate), str_buf);

    /* Semaphores */
    int sem_empty_cnt = 0, sem_full_cnt = 0;
    sem_getvalue(&ctx->empty_slots, &sem_empty_cnt);
    sem_getvalue(&ctx->full_slots, &sem_full_cnt);
    snprintf(str_buf, sizeof(str_buf), "%02d / %d SLOTS", sem_empty_cnt, BUFFER_CAPACITY);
    gtk_label_set_text(GTK_LABEL(gui->lbl_sem_empty), str_buf);
    snprintf(str_buf, sizeof(str_buf), "%02d / %d SLOTS", sem_full_cnt, BUFFER_CAPACITY);
    gtk_label_set_text(GTK_LABEL(gui->lbl_sem_full), str_buf);

    /* Mutex Contentions */
    snprintf(str_buf, sizeof(str_buf), "%lu EVENTS", (unsigned long)ctx->mutex_contentions);
    gtk_label_set_text(GTK_LABEL(gui->lbl_contentions), str_buf);

    /* Race Overwrite Errors */
    snprintf(str_buf, sizeof(str_buf), "%lu BLOCKS", (unsigned long)ctx->race_corrupted_blocks);
    gtk_label_set_text(GTK_LABEL(gui->lbl_race_errors), str_buf);

    GtkStyleContext *race_sc = gtk_widget_get_style_context(gui->lbl_race_errors);
    if (ctx->race_corrupted_blocks > 0) {
        gtk_style_context_remove_class(race_sc, "telemetry-val");
        gtk_style_context_add_class(race_sc, "telemetry-val-alert");
    } else {
        gtk_style_context_remove_class(race_sc, "telemetry-val-alert");
        gtk_style_context_add_class(race_sc, "telemetry-val");
    }

    /* Progress & Storage */
    snprintf(str_buf, sizeof(str_buf), "%.2f / %.2f MB", mb_downloaded, total_mb);
    gtk_label_set_text(GTK_LABEL(gui->lbl_bytes_downloaded), str_buf);

    snprintf(str_buf, sizeof(str_buf), "%.2f / %.2f MB", mb_written, total_mb);
    gtk_label_set_text(GTK_LABEL(gui->lbl_bytes_written), str_buf);

    double fraction = (total_mb > 0.0) ? (mb_written / total_mb) : 0.0;
    if (fraction > 1.0) fraction = 1.0;
    if (fraction < 0.0) fraction = 0.0;
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(gui->progress_bar), fraction);

    /* Update SHA-256 Widgets */
    if (ctx->expected_sha256[0]) {
        gtk_label_set_text(GTK_LABEL(gui->lbl_expected_sha), ctx->expected_sha256);
    }
    if (ctx->actual_sha256[0]) {
        gtk_label_set_text(GTK_LABEL(gui->lbl_actual_sha), ctx->actual_sha256);
    }

    /* Integrity Badge */
    GtkStyleContext *b_sc = gtk_widget_get_style_context(gui->lbl_integrity_badge);
    if (ctx->state == STATE_IDLE) {
        gtk_label_set_text(GTK_LABEL(gui->lbl_integrity_badge), "[ STATUS: STANDBY // IDLE ]");
        gtk_style_context_add_class(b_sc, "badge-idle");
        gtk_style_context_remove_class(b_sc, "badge-running");
        gtk_style_context_remove_class(b_sc, "badge-verified");
        gtk_style_context_remove_class(b_sc, "badge-corrupted");
    } else if (ctx->state == STATE_RUNNING) {
        gtk_label_set_text(GTK_LABEL(gui->lbl_integrity_badge), "[ STATUS: TRANSFER IN PROGRESS ]");
        gtk_style_context_add_class(b_sc, "badge-running");
        gtk_style_context_remove_class(b_sc, "badge-idle");
        gtk_style_context_remove_class(b_sc, "badge-verified");
        gtk_style_context_remove_class(b_sc, "badge-corrupted");
    } else if (ctx->state == STATE_COMPLETED) {
        gtk_style_context_remove_class(b_sc, "badge-idle");
        gtk_style_context_remove_class(b_sc, "badge-running");
        if (ctx->sha256_verified) {
            if (ctx->expected_sha256[0] != '\0') {
                gtk_label_set_text(GTK_LABEL(gui->lbl_integrity_badge), "[ STATUS: VERIFIED // BYTE-TO-BYTE INTEGRITY CONFIRMED ]");
            } else {
                gtk_label_set_text(GTK_LABEL(gui->lbl_integrity_badge), "[ STATUS: VERIFIED // EXTERNAL DOWNLOAD COMPLETE (0 RACE ERRORS) ]");
            }
            gtk_style_context_add_class(b_sc, "badge-verified");
            gtk_style_context_remove_class(b_sc, "badge-corrupted");
        } else {
            gtk_label_set_text(GTK_LABEL(gui->lbl_integrity_badge), "[ STATUS: CORRUPTED // DATA RACE DETECTED ]");
            gtk_style_context_add_class(b_sc, "badge-corrupted");
            gtk_style_context_remove_class(b_sc, "badge-verified");
        }
    }

    /* Feed Colorized Log Stream */
    pthread_mutex_lock(&ctx->log_mutex);
    if (ctx->log_count > gui->last_rendered_log_count) {
        for (int m = gui->last_rendered_log_count; m < ctx->log_count; m++) {
            int slot = (ctx->log_head + m) % LOG_BUFFER_CAPACITY;
            append_colorized_log_line(gui, ctx->log_messages[slot]);
        }
        gui->last_rendered_log_count = ctx->log_count;

        /* Auto-scroll to newest entry */
        GtkTextIter end_iter;
        gtk_text_buffer_get_end_iter(gui->text_buffer_logs, &end_iter);
        GtkTextMark *mark = gtk_text_buffer_create_mark(gui->text_buffer_logs, NULL, &end_iter, FALSE);
        gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(gui->text_view_logs), mark, 0.0, TRUE, 0.0, 1.0);
    }
    pthread_mutex_unlock(&ctx->log_mutex);

    /* Redraw Cairo visualizers */
    gtk_widget_queue_draw(gui->drawing_ram_buffer);
    gtk_widget_queue_draw(gui->drawing_block_matrix);
    gtk_widget_queue_draw(gui->drawing_sidebar_sparkline);

    return TRUE;
}

/* =========================================================================
 * ADAPTIVE CONCURRENCY TUNER (AUTO-OPTIMIZATION ENGINE)
 * ========================================================================= */
void auto_optimize_concurrency(GuiDashboard *gui) {
    if (!gui || !gui->auto_tune_enabled) return;
    AppContext *ctx = gui->ctx;

    bool is_local = (strncmp(ctx->target_url, "http://127.0.0.1", 16) == 0);
    int optimal_p = 4;
    int optimal_c = 2;

    if (is_local) {
        /* Local loopback: virtually 0 latency, high-bandwidth I/O */
        if (ctx->total_file_size <= 4 * 1024 * 1024) {
            optimal_p = 2; optimal_c = 1;
        } else if (ctx->total_file_size <= 16 * 1024 * 1024) {
            optimal_p = 4; optimal_c = 2;
        } else {
            optimal_p = 6; optimal_c = 2;
        }
    } else {
        /* Live Internet: Bandwidth-Delay Product (BDP) & Latency adaptive tuning */
        double size_mb = (double)ctx->total_file_size / (1024.0 * 1024.0);
        double rtt = (ctx->probe_rtt_ms > 0.0) ? ctx->probe_rtt_ms : 50.0;

        if (size_mb < 2.0) {
            optimal_p = (rtt > 80.0) ? 3 : 2;
            optimal_c = 1;
        } else if (size_mb < 15.0) {
            optimal_p = (rtt > 100.0) ? 6 : 4;
            optimal_c = 2;
        } else {
            optimal_p = (rtt > 60.0) ? 8 : 6;
            optimal_c = (size_mb > 50.0) ? 3 : 2;
        }
    }

    if (optimal_p > MAX_PRODUCER_THREADS) optimal_p = MAX_PRODUCER_THREADS;
    if (optimal_p < 1) optimal_p = 1;
    if (optimal_c > MAX_CONSUMER_THREADS) optimal_c = MAX_CONSUMER_THREADS;
    if (optimal_c < 1) optimal_c = 1;

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(gui->spin_producers), optimal_p);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(gui->spin_consumers), optimal_c);
    ctx->num_producers = optimal_p;
    ctx->num_consumers = optimal_c;

    app_log(ctx, "AUTO_TUNE", "CONFIG",
            "Target: %s (%.2f MB, RTT=%.1fms) -> Auto-optimized: %d Producers, %d Consumers",
            is_local ? "Local" : "Remote",
            (double)ctx->total_file_size / (1024.0 * 1024.0),
            ctx->probe_rtt_ms, optimal_p, optimal_c);
}

void on_btn_auto_tune_toggled(GtkToggleButton *button, gpointer user_data) {
    GuiDashboard *gui = (GuiDashboard *)user_data;
    gui->auto_tune_enabled = gtk_toggle_button_get_active(button);
    gui->ctx->auto_tune_enabled = gui->auto_tune_enabled;

    GtkStyleContext *sc = gtk_widget_get_style_context(GTK_WIDGET(button));
    if (gui->auto_tune_enabled) {
        gtk_button_set_label(GTK_BUTTON(button), "[AUTO-TUNE: ON]");
        gtk_style_context_remove_class(sc, "btn-auto-off");
        gtk_style_context_add_class(sc, "btn-auto-on");
        app_log(gui->ctx, "UI_ACTION", "CONFIG", "Adaptive Concurrency Tuner: ENABLED");
        if (gui->ctx->total_file_size > 0) {
            auto_optimize_concurrency(gui);
        }
    } else {
        gtk_button_set_label(GTK_BUTTON(button), "[AUTO-TUNE: OFF]");
        gtk_style_context_remove_class(sc, "btn-auto-on");
        gtk_style_context_add_class(sc, "btn-auto-off");
        app_log(gui->ctx, "UI_ACTION", "CONFIG", "Adaptive Concurrency Tuner: DISABLED (Manual)");
    }
}

/* =========================================================================
 * SIGNAL HANDLERS & TARGET SOURCE TELEMETRY
 * ========================================================================= */
static void update_target_source_ui(GuiDashboard *gui) {
    if (!gui || !gui->entry_url || !gui->lbl_source_badge) return;

    const char *text = gtk_entry_get_text(GTK_ENTRY(gui->entry_url));
    GtkStyleContext *b_test_sc = gtk_widget_get_style_context(gui->btn_test_offline);
    GtkStyleContext *badge_sc = gtk_widget_get_style_context(gui->lbl_source_badge);

    gtk_style_context_remove_class(b_test_sc, "btn-active");
    gtk_style_context_remove_class(badge_sc, "badge-dataset");
    gtk_style_context_remove_class(badge_sc, "badge-dataset-net");
    gtk_style_context_remove_class(badge_sc, "badge-idle");

    if (strlen(text) == 0) {
        gtk_label_set_text(GTK_LABEL(gui->lbl_source_badge), "[ TARGET: EMPTY ]");
        gtk_style_context_add_class(badge_sc, "badge-idle");
        if (gui->lbl_target_source) gtk_label_set_text(GTK_LABEL(gui->lbl_target_source), "NOT SELECTED");
    } else if (strstr(text, "benchmark_16mb.bin") || strstr(text, "127.0.0.1")) {
        gtk_label_set_text(GTK_LABEL(gui->lbl_source_badge), "[ TARGET: OFFLINE DEMO ]");
        gtk_style_context_add_class(badge_sc, "badge-dataset");
        gtk_style_context_add_class(b_test_sc, "btn-active");
        if (gui->lbl_target_source) gtk_label_set_text(GTK_LABEL(gui->lbl_target_source), "OFFLINE 16 MB");
    } else {
        gtk_label_set_text(GTK_LABEL(gui->lbl_source_badge), "[ TARGET: INTERNET URL ]");
        gtk_style_context_add_class(badge_sc, "badge-dataset-net");
        if (gui->lbl_target_source) gtk_label_set_text(GTK_LABEL(gui->lbl_target_source), "INTERNET URL");
        if (gui->ctx->expected_sha256[0] != '\0') {
            gui->ctx->expected_sha256[0] = '\0';
            gtk_label_set_text(GTK_LABEL(gui->lbl_expected_sha), "NONE (EXTERNAL INTERNET URL)");
        }
    }
}

void on_entry_url_changed(GtkEditable *editable, gpointer user_data) {
    (void)editable;
    GuiDashboard *gui = (GuiDashboard *)user_data;
    update_target_source_ui(gui);
}

void on_btn_start_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GuiDashboard *gui = (GuiDashboard *)user_data;
    AppContext *ctx = gui->ctx;

    if (gui->worker_running) return;

    const char *url = gtk_entry_get_text(GTK_ENTRY(gui->entry_url));
    if (strlen(url) == 0) {
        app_log(ctx, "UI_ACTION", "WARN", "Target URL is empty. Please click [TEST OFFLINE] or paste an external URL.");
        return;
    } else {
        strncpy(ctx->target_url, url, sizeof(ctx->target_url) - 1);
        if (strncmp(url, "http://127.0.0.1", 16) != 0) {
            ctx->expected_sha256[0] = '\0';
            gtk_label_set_text(GTK_LABEL(gui->lbl_expected_sha), "NONE (EXTERNAL INTERNET URL)");
        }
    }

    if (probe_remote_target(ctx, ctx->target_url) != 0) {
        app_log(ctx, "UI_ACTION", "ERROR", "Probe of target URL '%s' failed", ctx->target_url);
        return;
    }

    /* Auto-concurrency optimization or read manual spin settings */
    if (gui->auto_tune_enabled) {
        auto_optimize_concurrency(gui);
    } else {
        ctx->num_producers = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gui->spin_producers));
        ctx->num_consumers = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gui->spin_consumers));
    }

    if (strstr(ctx->target_url, "benchmark_16mb.bin") || strstr(ctx->target_url, "127.0.0.1")) {
        app_log(ctx, "UI_ACTION", "START", "Target: OFFLINE SYNTHETIC DATASET (16 MB, %d chunks, %dP/%dC)",
                ctx->total_chunks, ctx->num_producers, ctx->num_consumers);
    } else {
        app_log(ctx, "UI_ACTION", "START", "Target: LIVE INTERNET URL (%.2f MB, %d chunks, %dP/%dC)",
                (double)ctx->total_file_size / (1024.0 * 1024.0), ctx->total_chunks,
                ctx->num_producers, ctx->num_consumers);
    }

    gui->worker_running = true;
    pthread_create(&gui->engine_worker_thread, NULL, engine_coordinator_thread, gui);
}

void on_btn_pause_clicked(GtkButton *button, gpointer user_data) {
    GuiDashboard *gui = (GuiDashboard *)user_data;
    AppContext *ctx = gui->ctx;

    if (!gui->worker_running) return;

    pthread_mutex_lock(&ctx->control_mutex);
    ctx->is_paused = !ctx->is_paused;
    if (!ctx->is_paused) {
        pthread_cond_broadcast(&ctx->pause_cond);
        gtk_button_set_label(button, "PAUSE");
        app_log(ctx, "UI_ACTION", "CONTROL", "Pipeline resumed");
    } else {
        gtk_button_set_label(button, "RESUME");
        app_log(ctx, "UI_ACTION", "CONTROL", "Pipeline paused via user action");
    }
    pthread_mutex_unlock(&ctx->control_mutex);
}

void on_btn_reset_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GuiDashboard *gui = (GuiDashboard *)user_data;
    AppContext *ctx = gui->ctx;

    if (gui->worker_running) {
        ctx->stop_requested = true;
        pthread_cond_broadcast(&ctx->pause_cond);
        pthread_join(gui->engine_worker_thread, NULL);
        gui->worker_running = false;
    }

    app_context_reset(ctx);
    gui->last_rendered_log_count = 0;
    gui->max_speed_recorded = 0.0;
    memset(gui->speed_history, 0, sizeof(gui->speed_history));

    gtk_text_buffer_set_text(gui->text_buffer_logs, "", -1);
    gtk_label_set_text(GTK_LABEL(gui->lbl_actual_sha), "----------------------------------------------------------------");
    gtk_button_set_label(GTK_BUTTON(gui->btn_pause), "PAUSE");
    update_target_source_ui(gui);

    app_log(ctx, "UI_ACTION", "RESET", "Workspace reset to initial clean state");
}

void on_btn_test_offline_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GuiDashboard *gui = (GuiDashboard *)user_data;
    AppContext *ctx = gui->ctx;

    size_t size = 16 * 1024 * 1024;
    const char *path = "test_data/benchmark_16mb.bin";

    mkdir("test_data", 0755);
    app_log(ctx, "UI_ACTION", "GENERATE", "Preparing 16 MB offline benchmark dataset...");

    if (generate_benchmark_file(path, size, ctx->expected_sha256) != 0) {
        app_log(ctx, "UI_ACTION", "ERROR", "Failed to generate benchmark dataset");
        return;
    }

    int port = 0;
    start_local_http_server(ctx, path, &port);

    gtk_entry_set_text(GTK_ENTRY(gui->entry_url), ctx->target_url);
    gtk_label_set_text(GTK_LABEL(gui->lbl_expected_sha), ctx->expected_sha256);
    update_target_source_ui(gui);

    if (gui->auto_tune_enabled) {
        auto_optimize_concurrency(gui);
    }
}

void on_switch_sync_toggled(GtkSwitch *widget, gboolean state, gpointer user_data) {
    (void)widget;
    GuiDashboard *gui = (GuiDashboard *)user_data;
    AppContext *ctx = gui->ctx;

    ctx->sync_mode = state ? SYNC_MODE_SYNCHRONIZED : SYNC_MODE_UNSYNCHRONIZED;

    GtkStyleContext *sc = gtk_widget_get_style_context(gui->lbl_sync_mode);

    if (state) {
        gtk_label_set_text(GTK_LABEL(gui->lbl_sync_mode), "MODE: SYNCHRONIZED (MUTEX/SEM)");
        gtk_style_context_remove_class(sc, "mode-unsynced");
        gtk_style_context_add_class(sc, "mode-synced");
        app_log(ctx, "UI_ACTION", "CONFIG", "Concurrency mode changed to SYNCHRONIZED (Guarded with Mutex & Semaphores)");
    } else {
        gtk_label_set_text(GTK_LABEL(gui->lbl_sync_mode), "MODE: UNSYNCHRONIZED (RACE CONDITION)");
        gtk_style_context_remove_class(sc, "mode-synced");
        gtk_style_context_add_class(sc, "mode-unsynced");
        app_log(ctx, "UI_ACTION", "CONFIG", "Concurrency mode changed to UNSYNCHRONIZED (Race conditions active!)");
    }
}

/* =========================================================================
 * NAVIGATION & DOWNLOAD HISTORY CALLBACKS
 * ========================================================================= */
void on_nav_workspace_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GuiDashboard *gui = (GuiDashboard *)user_data;
    gtk_stack_set_visible_child_name(GTK_STACK(gui->main_stack), "workspace");

    GtkStyleContext *sc_w = gtk_widget_get_style_context(gui->btn_nav_workspace);
    GtkStyleContext *sc_h = gtk_widget_get_style_context(gui->btn_nav_history);
    gtk_style_context_add_class(sc_w, "nav-item-active");
    gtk_style_context_remove_class(sc_h, "nav-item-active");
}

void on_nav_history_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GuiDashboard *gui = (GuiDashboard *)user_data;
    gtk_stack_set_visible_child_name(GTK_STACK(gui->main_stack), "history");

    GtkStyleContext *sc_w = gtk_widget_get_style_context(gui->btn_nav_workspace);
    GtkStyleContext *sc_h = gtk_widget_get_style_context(gui->btn_nav_history);
    gtk_style_context_remove_class(sc_w, "nav-item-active");
    gtk_style_context_add_class(sc_h, "nav-item-active");
}

void on_btn_open_storage_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;
    mkdir("test_data", 0755);
    int ret = system("xdg-open test_data/ >/dev/null 2>&1 &");
    (void)ret;
}

void on_btn_clear_history_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GuiDashboard *gui = (GuiDashboard *)user_data;
    if (gui->history_store) {
        gtk_list_store_clear(gui->history_store);
    }
    gui->history_count = 0;
    if (gui->lbl_hist_total) gtk_label_set_text(GTK_LABEL(gui->lbl_hist_total), "0");
    if (gui->lbl_hist_verified) gtk_label_set_text(GTK_LABEL(gui->lbl_hist_verified), "0");
    if (gui->lbl_hist_corrupted) gtk_label_set_text(GTK_LABEL(gui->lbl_hist_corrupted), "0");
    if (gui->lbl_hist_volume) gtk_label_set_text(GTK_LABEL(gui->lbl_hist_volume), "0.00 MB");
    if (gui->lbl_nav_hist_badge) gtk_label_set_text(GTK_LABEL(gui->lbl_nav_hist_badge), "0");
    app_log(gui->ctx, "HISTORY", "CLEAR", "Download session history wiped.");
}

void on_btn_copy_sha_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GuiDashboard *gui = (GuiDashboard *)user_data;
    if (!gui || !gui->tree_history) return;

    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(gui->tree_history));
    GtkTreeModel *model;
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(sel, &model, &iter)) {
        gchar *sha = NULL;
        gtk_tree_model_get(model, &iter, 5, &sha, -1);
        if (sha) {
            GtkClipboard *cb = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
            gtk_clipboard_set_text(cb, sha, -1);
            app_log(gui->ctx, "HISTORY", "CLIPBOARD", "Copied SHA-256 to clipboard: %s", sha);
            g_free(sha);
        }
    }
}

static void on_tree_history_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data) {
    (void)tree_view;
    (void)path;
    (void)column;
    on_btn_open_storage_clicked(NULL, user_data);
}

void gui_add_history_record(GuiDashboard *gui, const DownloadHistoryRecord *rec) {
    if (!gui || !rec) return;

    if (gui->history_count < MAX_HISTORY_RECORDS) {
        gui->history_items[gui->history_count] = *rec;
        gui->history_count++;
    }

    char size_buf[32];
    if (rec->file_size >= 1024 * 1024) {
        snprintf(size_buf, sizeof(size_buf), "%.2f MB", rec->file_size / (1024.0 * 1024.0));
    } else if (rec->file_size >= 1024) {
        snprintf(size_buf, sizeof(size_buf), "%.1f KB", rec->file_size / 1024.0);
    } else {
        snprintf(size_buf, sizeof(size_buf), "%zu B", rec->file_size);
    }

    char mode_buf[32];
    snprintf(mode_buf, sizeof(mode_buf), "%s", 
             (rec->sync_mode == SYNC_MODE_SYNCHRONIZED) ? "SYNC (MUTEX)" : "UNSYNC (RACE)");

    char status_buf[48];
    if (rec->file_size == 0 || strcmp(rec->filename, "DOWNLOAD_FAILED") == 0) {
        snprintf(status_buf, sizeof(status_buf), "⚠ NETWORK ERROR");
    } else if (rec->is_verified && rec->race_errors == 0) {
        snprintf(status_buf, sizeof(status_buf), "✔ VERIFIED [OK]");
    } else {
        snprintf(status_buf, sizeof(status_buf), "✖ RACE CORRUPTED (%lu)", (unsigned long)rec->race_errors);
    }

    char speed_buf[48];
    snprintf(speed_buf, sizeof(speed_buf), "%.2fs (%.2f MB/s)", rec->duration_sec, rec->speed_mbps);

    if (gui->history_store) {
        GtkTreeIter iter;
        gtk_list_store_prepend(gui->history_store, &iter);
        gtk_list_store_set(gui->history_store, &iter,
                           0, rec->timestamp,
                           1, rec->filename,
                           2, size_buf,
                           3, mode_buf,
                           4, status_buf,
                           5, rec->sha256,
                           6, speed_buf,
                           -1);
    }

    /* Recalculate summary metrics */
    int total = gui->history_count;
    int verified = 0;
    int corrupted = 0;
    size_t total_bytes = 0;

    for (int i = 0; i < gui->history_count; i++) {
        if (gui->history_items[i].is_verified && gui->history_items[i].race_errors == 0 && gui->history_items[i].file_size > 0) {
            verified++;
        } else {
            corrupted++;
        }
        total_bytes += gui->history_items[i].file_size;
    }

    char buf[64];
    snprintf(buf, sizeof(buf), "%d", total);
    if (gui->lbl_hist_total) gtk_label_set_text(GTK_LABEL(gui->lbl_hist_total), buf);
    if (gui->lbl_nav_hist_badge) gtk_label_set_text(GTK_LABEL(gui->lbl_nav_hist_badge), buf);

    snprintf(buf, sizeof(buf), "%d", verified);
    if (gui->lbl_hist_verified) gtk_label_set_text(GTK_LABEL(gui->lbl_hist_verified), buf);

    snprintf(buf, sizeof(buf), "%d", corrupted);
    if (gui->lbl_hist_corrupted) gtk_label_set_text(GTK_LABEL(gui->lbl_hist_corrupted), buf);

    snprintf(buf, sizeof(buf), "%.2f MB", total_bytes / (1024.0 * 1024.0));
    if (gui->lbl_hist_volume) gtk_label_set_text(GTK_LABEL(gui->lbl_hist_volume), buf);
}

/* =========================================================================
 * NAVIGATION SIDEBAR CONSTRUCTOR (PAGE SWITCHER)
 * ========================================================================= */
static GtkWidget* create_navigation_sidebar(GuiDashboard *gui) {
    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(sidebar, 215, -1);
    gtk_style_context_add_class(gtk_widget_get_style_context(sidebar), "nav-sidebar");

    /* App Header / Brand */
    GtkWidget *brand_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_box_pack_start(GTK_BOX(sidebar), brand_box, FALSE, FALSE, 4);

    GtkWidget *lbl_brand = gtk_label_new("CONCURRENCY LAB");
    gtk_widget_set_halign(lbl_brand, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_brand), "nav-brand-title");
    gtk_box_pack_start(GTK_BOX(brand_box), lbl_brand, FALSE, FALSE, 0);

    GtkWidget *lbl_sub = gtk_label_new("POSIX THREADS v2.0");
    gtk_widget_set_halign(lbl_sub, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_sub), "nav-brand-subtitle");
    gtk_box_pack_start(GTK_BOX(brand_box), lbl_sub, FALSE, FALSE, 0);

    /* Separator */
    GtkWidget *sep_top = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(sidebar), sep_top, FALSE, FALSE, 6);

    /* Section label */
    GtkWidget *lbl_sec = gtk_label_new("WORKSPACES");
    gtk_widget_set_halign(lbl_sec, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_sec), "nav-section-label");
    gtk_box_pack_start(GTK_BOX(sidebar), lbl_sec, FALSE, FALSE, 2);

    /* Menu Item 1: TRANSFER ENGINE (Active by default) */
    gui->btn_nav_workspace = gtk_button_new();
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_nav_workspace), "nav-item");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_nav_workspace), "nav-item-active");

    GtkWidget *box_item1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget *icon1 = gtk_label_new("⚡");
    GtkWidget *lbl_item1 = gtk_label_new("Transfer Engine");
    gtk_box_pack_start(GTK_BOX(box_item1), icon1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_item1), lbl_item1, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(gui->btn_nav_workspace), box_item1);
    g_signal_connect(gui->btn_nav_workspace, "clicked", G_CALLBACK(on_nav_workspace_clicked), gui);
    gtk_box_pack_start(GTK_BOX(sidebar), gui->btn_nav_workspace, FALSE, FALSE, 3);

    /* Menu Item 2: DOWNLOAD HISTORY */
    gui->btn_nav_history = gtk_button_new();
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_nav_history), "nav-item");

    GtkWidget *box_item2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget *icon2 = gtk_label_new("📋");
    GtkWidget *lbl_item2 = gtk_label_new("Download History");
    gtk_box_pack_start(GTK_BOX(box_item2), icon2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_item2), lbl_item2, TRUE, TRUE, 0);

    gui->lbl_nav_hist_badge = gtk_label_new("0");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_nav_hist_badge), "nav-badge-count");
    gtk_box_pack_end(GTK_BOX(box_item2), gui->lbl_nav_hist_badge, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(gui->btn_nav_history), box_item2);
    g_signal_connect(gui->btn_nav_history, "clicked", G_CALLBACK(on_nav_history_clicked), gui);
    gtk_box_pack_start(GTK_BOX(sidebar), gui->btn_nav_history, FALSE, FALSE, 3);

    /* Vertical Spacer to push quick actions to bottom */
    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(sidebar), spacer, TRUE, TRUE, 0);

    /* Bottom Quick Action: Open test_data/ Storage Folder */
    GtkWidget *btn_open_storage = gtk_button_new_with_label("📁 Mở thư mục tải về");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_open_storage), "btn-secondary");
    g_signal_connect(btn_open_storage, "clicked", G_CALLBACK(on_btn_open_storage_clicked), gui);
    gtk_box_pack_start(GTK_BOX(sidebar), btn_open_storage, FALSE, FALSE, 4);

    /* Footer info */
    GtkWidget *footer_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_style_context_add_class(gtk_widget_get_style_context(footer_box), "nav-footer-box");
    gtk_box_pack_start(GTK_BOX(sidebar), footer_box, FALSE, FALSE, 0);

    GtkWidget *lbl_f1 = gtk_label_new("TARGET: test_data/");
    gtk_widget_set_halign(lbl_f1, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_f1), "nav-footer-text");
    gtk_box_pack_start(GTK_BOX(footer_box), lbl_f1, FALSE, FALSE, 0);

    GtkWidget *lbl_f2 = gtk_label_new("POSIX ARCH: Linux 64-bit");
    gtk_widget_set_halign(lbl_f2, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_f2), "nav-footer-text");
    gtk_box_pack_start(GTK_BOX(footer_box), lbl_f2, FALSE, FALSE, 0);

    return sidebar;
}

/* =========================================================================
 * PAGE 2: DOWNLOAD HISTORY & INTEGRITY AUDIT VIEW
 * ========================================================================= */
static GtkWidget* create_history_page(GuiDashboard *gui) {
    GtkWidget *hist_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_style_context_add_class(gtk_widget_get_style_context(hist_box), "history-page");

    /* Header */
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_box_pack_start(GTK_BOX(hist_box), header_box, FALSE, FALSE, 0);

    GtkWidget *lbl_title = gtk_label_new("DOWNLOAD SESSION HISTORY & INTEGRITY AUDIT");
    gtk_widget_set_halign(lbl_title, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_title), "history-header-title");
    gtk_box_pack_start(GTK_BOX(header_box), lbl_title, FALSE, FALSE, 0);

    GtkWidget *lbl_desc = gtk_label_new("Log of multi-threaded transfers, SHA-256 cryptographic verification, and Race Condition audit records.");
    gtk_widget_set_halign(lbl_desc, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_desc), "history-header-desc");
    gtk_box_pack_start(GTK_BOX(header_box), lbl_desc, FALSE, FALSE, 0);

    /* Metric Cards Grid */
    GtkWidget *metrics_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_box_pack_start(GTK_BOX(hist_box), metrics_box, FALSE, FALSE, 0);

    #define MAKE_HIST_CARD(parent, title_text, val_widget, val_class) do { \
        GtkWidget *card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4); \
        gtk_style_context_add_class(gtk_widget_get_style_context(card), "history-card-metric"); \
        GtkWidget *t = gtk_label_new(title_text); \
        gtk_widget_set_halign(t, GTK_ALIGN_START); \
        gtk_style_context_add_class(gtk_widget_get_style_context(t), "history-metric-title"); \
        gtk_box_pack_start(GTK_BOX(card), t, FALSE, FALSE, 0); \
        gtk_widget_set_halign(val_widget, GTK_ALIGN_START); \
        gtk_style_context_add_class(gtk_widget_get_style_context(val_widget), val_class); \
        gtk_box_pack_start(GTK_BOX(card), val_widget, FALSE, FALSE, 0); \
        gtk_box_pack_start(GTK_BOX(parent), card, TRUE, TRUE, 0); \
    } while(0)

    gui->lbl_hist_total = gtk_label_new("0");
    MAKE_HIST_CARD(metrics_box, "TOTAL SESSIONS", gui->lbl_hist_total, "history-metric-val");

    gui->lbl_hist_verified = gtk_label_new("0");
    MAKE_HIST_CARD(metrics_box, "VERIFIED SAFE (SHA-256)", gui->lbl_hist_verified, "history-metric-val-ok");

    gui->lbl_hist_corrupted = gtk_label_new("0");
    MAKE_HIST_CARD(metrics_box, "RACE CORRUPTED / FAILED", gui->lbl_hist_corrupted, "history-metric-val-err");

    gui->lbl_hist_volume = gtk_label_new("0.00 MB");
    MAKE_HIST_CARD(metrics_box, "TOTAL VOLUME DOWNLOADED", gui->lbl_hist_volume, "history-metric-val");

    /* Action Toolbar for History */
    GtkWidget *act_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(hist_box), act_bar, FALSE, FALSE, 0);

    GtkWidget *btn_open_storage = gtk_button_new_with_label("📁 Mở thư mục tải về (test_data/)");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_open_storage), "btn-steel");
    g_signal_connect(btn_open_storage, "clicked", G_CALLBACK(on_btn_open_storage_clicked), gui);
    gtk_box_pack_start(GTK_BOX(act_bar), btn_open_storage, FALSE, FALSE, 0);

    GtkWidget *btn_copy_sha = gtk_button_new_with_label("📋 Sao chép mã SHA-256");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_copy_sha), "btn-secondary");
    g_signal_connect(btn_copy_sha, "clicked", G_CALLBACK(on_btn_copy_sha_clicked), gui);
    gtk_box_pack_start(GTK_BOX(act_bar), btn_copy_sha, FALSE, FALSE, 0);

    GtkWidget *btn_clear = gtk_button_new_with_label("🗑 Xóa lịch sử");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_clear), "btn-secondary");
    g_signal_connect(btn_clear, "clicked", G_CALLBACK(on_btn_clear_history_clicked), gui);
    gtk_box_pack_end(GTK_BOX(act_bar), btn_clear, FALSE, FALSE, 0);

    /* Scrolled TreeView Table */
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(hist_box), scrolled, TRUE, TRUE, 0);

    gui->history_store = gtk_list_store_new(7, 
                                            G_TYPE_STRING, /* 0: Timestamp */
                                            G_TYPE_STRING, /* 1: Filename */
                                            G_TYPE_STRING, /* 2: Size */
                                            G_TYPE_STRING, /* 3: Mode */
                                            G_TYPE_STRING, /* 4: Status */
                                            G_TYPE_STRING, /* 5: SHA-256 */
                                            G_TYPE_STRING  /* 6: Speed & Time */
                                            );
    gui->tree_history = gtk_tree_view_new_with_model(GTK_TREE_MODEL(gui->history_store));
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->tree_history), "history-tree");
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(gui->tree_history), TRUE);
    g_signal_connect(gui->tree_history, "row-activated", G_CALLBACK(on_tree_history_row_activated), gui);

    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *col;

    col = gtk_tree_view_column_new_with_attributes("THỜI GIAN", renderer, "text", 0, NULL);
    gtk_tree_view_column_set_resizable(col, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(gui->tree_history), col);

    col = gtk_tree_view_column_new_with_attributes("TÊN TỆP TIN", renderer, "text", 1, NULL);
    gtk_tree_view_column_set_resizable(col, TRUE);
    gtk_tree_view_column_set_min_width(col, 200);
    gtk_tree_view_append_column(GTK_TREE_VIEW(gui->tree_history), col);

    col = gtk_tree_view_column_new_with_attributes("DUNG LƯỢNG", renderer, "text", 2, NULL);
    gtk_tree_view_column_set_resizable(col, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(gui->tree_history), col);

    col = gtk_tree_view_column_new_with_attributes("CHẾ ĐỘ", renderer, "text", 3, NULL);
    gtk_tree_view_column_set_resizable(col, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(gui->tree_history), col);

    col = gtk_tree_view_column_new_with_attributes("TRẠNG THÁI KIỂM TOÁN", renderer, "text", 4, NULL);
    gtk_tree_view_column_set_resizable(col, TRUE);
    gtk_tree_view_column_set_min_width(col, 160);
    gtk_tree_view_append_column(GTK_TREE_VIEW(gui->tree_history), col);

    GtkCellRenderer *sha_renderer = gtk_cell_renderer_text_new();
    g_object_set(sha_renderer, "family", "monospace", NULL);
    col = gtk_tree_view_column_new_with_attributes("MÃ SHA-256 THỰC TẾ", sha_renderer, "text", 5, NULL);
    gtk_tree_view_column_set_resizable(col, TRUE);
    gtk_tree_view_column_set_min_width(col, 280);
    gtk_tree_view_append_column(GTK_TREE_VIEW(gui->tree_history), col);

    col = gtk_tree_view_column_new_with_attributes("TỐC ĐỘ / THỜI GIAN", renderer, "text", 6, NULL);
    gtk_tree_view_column_set_resizable(col, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(gui->tree_history), col);

    gtk_container_add(GTK_CONTAINER(scrolled), gui->tree_history);

    return hist_box;
}

/* =========================================================================
 * DASHBOARD INITIALIZATION & WIDGET TREE CONSTRUCTION
 * ========================================================================= */
int gui_dashboard_create(GuiDashboard *gui, AppContext *ctx, int argc, char **argv) {
    gtk_init(&argc, &argv);

    gui->ctx = ctx;
    gui->worker_running = false;
    gui->auto_tune_enabled = true;
    gui->last_rendered_log_count = 0;
    gui->max_speed_recorded = 0.0;
    memset(gui->speed_history, 0, sizeof(gui->speed_history));

    /* Load Industrial Matte Carbon & Oxide CSS */
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider, WORKSTATION_CSS, -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(css_provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    /* Main Window: 1300 x 820 */
    gui->main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(gui->main_window),
                         "SYSTEM PROGRAMMING WORKSTATION // MULTI-THREADED DOWNLOAD MANAGER & CONCURRENCY LAB");
    gtk_window_set_default_size(GTK_WINDOW(gui->main_window), 1300, 820);
    gtk_window_set_position(GTK_WINDOW(gui->main_window), GTK_WIN_POS_CENTER);
    g_signal_connect(gui->main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Root Horizontal Box: Sidebar on left, Stack on right */
    GtkWidget *root_h_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(gui->main_window), root_h_box);

    /* 1. Left Navigation Sidebar */
    GtkWidget *nav_sidebar = create_navigation_sidebar(gui);
    gtk_box_pack_start(GTK_BOX(root_h_box), nav_sidebar, FALSE, FALSE, 0);

    /* 2. Main Stack Container */
    gui->main_stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(gui->main_stack), GTK_STACK_TRANSITION_TYPE_CROSSFADE);
    gtk_stack_set_transition_duration(GTK_STACK(gui->main_stack), 150);
    gtk_box_pack_start(GTK_BOX(root_h_box), gui->main_stack, TRUE, TRUE, 0);

    /* =====================================================================
     * PAGE 1: TRANSFER ENGINE WORKSPACE
     * ===================================================================== */
    gui->page_workspace = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_stack_add_named(GTK_STACK(gui->main_stack), gui->page_workspace, "workspace");

    /* =====================================================================
     * 1. TOP TOOLBAR (FLAT HIGH-DENSITY WORKSTATION HEADER)
     * ===================================================================== */
    GtkWidget *toolbar_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_style_context_add_class(gtk_widget_get_style_context(toolbar_box), "top-toolbar");
    gtk_box_pack_start(GTK_BOX(gui->page_workspace), toolbar_box, FALSE, FALSE, 0);

    /* URL Entry */
    GtkWidget *lbl_url = gtk_label_new("TARGET URL:");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_url), "section-header");
    gtk_box_pack_start(GTK_BOX(toolbar_box), lbl_url, FALSE, FALSE, 0);

    gui->entry_url = gtk_entry_new();
    gtk_widget_set_size_request(gui->entry_url, 360, 28);
    gtk_entry_set_placeholder_text(GTK_ENTRY(gui->entry_url), "Paste URL (e.g. https://... or click [TEST OFFLINE])");
    gtk_box_pack_start(GTK_BOX(toolbar_box), gui->entry_url, TRUE, TRUE, 0);

    gui->lbl_source_badge = gtk_label_new("[ TARGET: EMPTY ]");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_source_badge), "badge-idle");
    gtk_box_pack_start(GTK_BOX(toolbar_box), gui->lbl_source_badge, FALSE, FALSE, 0);

    g_signal_connect(gui->entry_url, "changed", G_CALLBACK(on_entry_url_changed), gui);

    /* Single Consolidated Offline Benchmark Button */
    gui->btn_test_offline = gtk_button_new_with_label("[TEST OFFLINE]");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_test_offline), "btn-steel");
    g_signal_connect(gui->btn_test_offline, "clicked", G_CALLBACK(on_btn_test_offline_clicked), gui);
    gtk_box_pack_start(GTK_BOX(toolbar_box), gui->btn_test_offline, FALSE, FALSE, 0);

    /* Separator */
    GtkWidget *sep1 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_start(GTK_BOX(toolbar_box), sep1, FALSE, FALSE, 4);

    /* Auto-Tune Adaptive Concurrency Button */
    gui->btn_auto_tune = gtk_toggle_button_new_with_label("[AUTO-TUNE: ON]");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui->btn_auto_tune), TRUE);
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_auto_tune), "btn-auto-on");
    g_signal_connect(gui->btn_auto_tune, "toggled", G_CALLBACK(on_btn_auto_tune_toggled), gui);
    gtk_box_pack_start(GTK_BOX(toolbar_box), gui->btn_auto_tune, FALSE, FALSE, 0);

    /* Producers SpinButton */
    GtkWidget *lbl_p = gtk_label_new("PRODUCERS:");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_p), "section-header");
    gtk_box_pack_start(GTK_BOX(toolbar_box), lbl_p, FALSE, FALSE, 0);

    gui->spin_producers = gtk_spin_button_new_with_range(1, 8, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(gui->spin_producers), 4);
    gtk_box_pack_start(GTK_BOX(toolbar_box), gui->spin_producers, FALSE, FALSE, 0);

    /* Consumers SpinButton */
    GtkWidget *lbl_c = gtk_label_new("CONSUMERS:");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_c), "section-header");
    gtk_box_pack_start(GTK_BOX(toolbar_box), lbl_c, FALSE, FALSE, 0);

    gui->spin_consumers = gtk_spin_button_new_with_range(1, 8, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(gui->spin_consumers), 2);
    gtk_box_pack_start(GTK_BOX(toolbar_box), gui->spin_consumers, FALSE, FALSE, 0);

    /* Mode Switch */
    gui->switch_sync_mode = gtk_switch_new();
    gtk_switch_set_active(GTK_SWITCH(gui->switch_sync_mode), TRUE);
    g_signal_connect(gui->switch_sync_mode, "state-set", G_CALLBACK(on_switch_sync_toggled), gui);
    gtk_box_pack_start(GTK_BOX(toolbar_box), gui->switch_sync_mode, FALSE, FALSE, 0);

    gui->lbl_sync_mode = gtk_label_new("MODE: SYNCHRONIZED (MUTEX/SEM)");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_sync_mode), "mode-synced");
    gtk_box_pack_start(GTK_BOX(toolbar_box), gui->lbl_sync_mode, FALSE, FALSE, 0);

    /* Control Buttons */
    gui->btn_start = gtk_button_new_with_label("START TRANSFER");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_start), "btn-primary");
    g_signal_connect(gui->btn_start, "clicked", G_CALLBACK(on_btn_start_clicked), gui);
    gtk_box_pack_start(GTK_BOX(toolbar_box), gui->btn_start, FALSE, FALSE, 0);

    gui->btn_pause = gtk_button_new_with_label("PAUSE");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_pause), "btn-secondary");
    g_signal_connect(gui->btn_pause, "clicked", G_CALLBACK(on_btn_pause_clicked), gui);
    gtk_box_pack_start(GTK_BOX(toolbar_box), gui->btn_pause, FALSE, FALSE, 0);

    gui->btn_reset = gtk_button_new_with_label("RESET");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_reset), "btn-secondary");
    g_signal_connect(gui->btn_reset, "clicked", G_CALLBACK(on_btn_reset_clicked), gui);
    gtk_box_pack_start(GTK_BOX(toolbar_box), gui->btn_reset, FALSE, FALSE, 0);

    /* =====================================================================
     * 2. MAIN HORIZONTAL SPLIT: SIDEBAR (LEFT) + WORKSPACE (RIGHT)
     * ===================================================================== */
    GtkWidget *body_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(gui->page_workspace), body_box, TRUE, TRUE, 0);

    /* ---------------------------------------------------------------------
     * LEFT SIDEBAR: SYSTEM TELEMETRY & SPARKLINE GRAPH (WIDTH: 270px)
     * --------------------------------------------------------------------- */
    GtkWidget *sidebar_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_size_request(sidebar_box, 270, -1);
    gtk_style_context_add_class(gtk_widget_get_style_context(sidebar_box), "sidebar-panel");
    gtk_box_pack_start(GTK_BOX(body_box), sidebar_box, FALSE, FALSE, 0);

    GtkWidget *lbl_side_title = gtk_label_new("SYSTEM TELEMETRY");
    gtk_widget_set_halign(lbl_side_title, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_side_title), "section-header");
    gtk_box_pack_start(GTK_BOX(sidebar_box), lbl_side_title, FALSE, FALSE, 0);

    /* Card 1: Engine State & Transfer Rate */
    GtkWidget *card_speed = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_style_context_add_class(gtk_widget_get_style_context(card_speed), "telemetry-card");
    gtk_box_pack_start(GTK_BOX(sidebar_box), card_speed, FALSE, FALSE, 0);

    GtkWidget *lbl_rate_hdr = gtk_label_new("INSTANTANEOUS THROUGHPUT");
    gtk_widget_set_halign(lbl_rate_hdr, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_rate_hdr), "telemetry-key");
    gtk_box_pack_start(GTK_BOX(card_speed), lbl_rate_hdr, FALSE, FALSE, 0);

    gui->lbl_rate = gtk_label_new("  0.00 MB/s");
    gtk_widget_set_halign(gui->lbl_rate, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_rate), "telemetry-val-large");
    gtk_box_pack_start(GTK_BOX(card_speed), gui->lbl_rate, FALSE, FALSE, 0);

    /* Card 2: Real-time Throughput Sparkline Graph */
    gui->drawing_sidebar_sparkline = gtk_drawing_area_new();
    gtk_widget_set_size_request(gui->drawing_sidebar_sparkline, -1, 60);
    g_signal_connect(gui->drawing_sidebar_sparkline, "draw", G_CALLBACK(on_draw_sidebar_sparkline), gui);
    gtk_box_pack_start(GTK_BOX(sidebar_box), gui->drawing_sidebar_sparkline, FALSE, FALSE, 0);

    /* Card 3: Semaphore State */
    GtkWidget *card_sem = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_style_context_add_class(gtk_widget_get_style_context(card_sem), "telemetry-card");
    gtk_box_pack_start(GTK_BOX(sidebar_box), card_sem, FALSE, FALSE, 0);

    #define MAKE_KEYVAL_ROW(k_text, val_lbl) do { \
        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8); \
        GtkWidget *k = gtk_label_new(k_text); \
        gtk_widget_set_halign(k, GTK_ALIGN_START); \
        gtk_style_context_add_class(gtk_widget_get_style_context(k), "telemetry-key"); \
        gtk_box_pack_start(GTK_BOX(row), k, FALSE, FALSE, 0); \
        gtk_widget_set_halign(val_lbl, GTK_ALIGN_END); \
        gtk_style_context_add_class(gtk_widget_get_style_context(val_lbl), "telemetry-val"); \
        gtk_box_pack_end(GTK_BOX(row), val_lbl, FALSE, FALSE, 0); \
        gtk_box_pack_start(GTK_BOX(card_sem), row, FALSE, FALSE, 0); \
    } while(0)

    gui->lbl_engine_state = gtk_label_new("STANDBY [IDLE]");
    MAKE_KEYVAL_ROW("PIPELINE STATE", gui->lbl_engine_state);

    gui->lbl_sem_empty = gtk_label_new("32 / 32 SLOTS");
    MAKE_KEYVAL_ROW("SEM_EMPTY", gui->lbl_sem_empty);

    gui->lbl_sem_full = gtk_label_new("00 / 32 SLOTS");
    MAKE_KEYVAL_ROW("SEM_FULL", gui->lbl_sem_full);

    gui->lbl_contentions = gtk_label_new("0 EVENTS");
    MAKE_KEYVAL_ROW("MUTEX LOCK COLLISIONS", gui->lbl_contentions);

    gui->lbl_race_errors = gtk_label_new("0 BLOCKS");
    MAKE_KEYVAL_ROW("RACE OVERWRITES", gui->lbl_race_errors);

    /* Card 4: Storage Progress */
    GtkWidget *card_prog = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_style_context_add_class(gtk_widget_get_style_context(card_prog), "telemetry-card");
    gtk_box_pack_start(GTK_BOX(sidebar_box), card_prog, FALSE, FALSE, 0);

    gui->lbl_bytes_downloaded = gtk_label_new("0.00 / 0.00 MB");
    MAKE_KEYVAL_ROW("DOWNLOADED", gui->lbl_bytes_downloaded);

    gui->lbl_bytes_written = gtk_label_new("0.00 / 0.00 MB");
    MAKE_KEYVAL_ROW("COMMITTED", gui->lbl_bytes_written);

    gui->progress_bar = gtk_progress_bar_new();
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(gui->progress_bar), 0.0);
    gtk_box_pack_start(GTK_BOX(card_prog), gui->progress_bar, FALSE, FALSE, 4);

    /* Card 5: Static Topology */
    GtkWidget *card_topo = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_style_context_add_class(gtk_widget_get_style_context(card_topo), "telemetry-card");
    gtk_box_pack_start(GTK_BOX(sidebar_box), card_topo, FALSE, FALSE, 0);

    #define MAKE_TOPO_ROW(k_text, v_text) do { \
        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8); \
        GtkWidget *k = gtk_label_new(k_text); \
        gtk_widget_set_halign(k, GTK_ALIGN_START); \
        gtk_style_context_add_class(gtk_widget_get_style_context(k), "telemetry-key"); \
        gtk_box_pack_start(GTK_BOX(row), k, FALSE, FALSE, 0); \
        GtkWidget *v = gtk_label_new(v_text); \
        gtk_widget_set_halign(v, GTK_ALIGN_END); \
        gtk_style_context_add_class(gtk_widget_get_style_context(v), "telemetry-val"); \
        gtk_box_pack_end(GTK_BOX(row), v, FALSE, FALSE, 0); \
        gtk_box_pack_start(GTK_BOX(card_topo), row, FALSE, FALSE, 0); \
    } while(0)

    GtkWidget *row_src = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget *k_src = gtk_label_new("ACTIVE TARGET");
    gtk_widget_set_halign(k_src, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(k_src), "telemetry-key");
    gtk_box_pack_start(GTK_BOX(row_src), k_src, FALSE, FALSE, 0);
    gui->lbl_target_source = gtk_label_new("NOT SELECTED");
    gtk_widget_set_halign(gui->lbl_target_source, GTK_ALIGN_END);
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_target_source), "telemetry-val");
    gtk_box_pack_end(GTK_BOX(row_src), gui->lbl_target_source, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(card_topo), row_src, FALSE, FALSE, 0);

    MAKE_TOPO_ROW("CHUNK SIZE", "64 KB (65,536 B)");
    MAKE_TOPO_ROW("BUFFER FOOTPRINT", "32 SLOTS (2.0 MB)");
    MAKE_TOPO_ROW("STORAGE DIR", "test_data/");
    MAKE_TOPO_ROW("CONCURRENCY", "ADAPTIVE AUTO-TUNE");
    MAKE_TOPO_ROW("HASH ALGORITHM", "SHA-256 (EVP)");

    /* ---------------------------------------------------------------------
     * CENTER WORKSPACE: 2 CAIRO 2D INSPECTORS
     * --------------------------------------------------------------------- */
    GtkWidget *workspace_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_style_context_add_class(gtk_widget_get_style_context(workspace_box), "workspace-panel");
    gtk_box_pack_start(GTK_BOX(body_box), workspace_box, TRUE, TRUE, 0);

    /* Top Cairo: RAM Circular Bounded Buffer */
    GtkWidget *lbl_cairo_top = gtk_label_new("RAM BOUNDED BUFFER INSPECTOR // 32 SLOTS MEMORY GEOMETRY");
    gtk_widget_set_halign(lbl_cairo_top, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_cairo_top), "section-header");
    gtk_box_pack_start(GTK_BOX(workspace_box), lbl_cairo_top, FALSE, FALSE, 0);

    gui->drawing_ram_buffer = gtk_drawing_area_new();
    gtk_widget_set_size_request(gui->drawing_ram_buffer, -1, 115);
    g_signal_connect(gui->drawing_ram_buffer, "draw", G_CALLBACK(on_draw_ram_buffer), gui);
    gtk_box_pack_start(GTK_BOX(workspace_box), gui->drawing_ram_buffer, FALSE, FALSE, 0);

    /* Bottom Cairo: Disk Block Allocation Matrix */
    GtkWidget *lbl_cairo_bot = gtk_label_new("DISK BLOCK ALLOCATION MATRIX // 256 BLOCKS (16-32 MB STORAGE MAP)");
    gtk_widget_set_halign(lbl_cairo_bot, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_cairo_bot), "section-header");
    gtk_box_pack_start(GTK_BOX(workspace_box), lbl_cairo_bot, FALSE, FALSE, 4);

    gui->drawing_block_matrix = gtk_drawing_area_new();
    gtk_widget_set_size_request(gui->drawing_block_matrix, -1, 195);
    g_signal_connect(gui->drawing_block_matrix, "draw", G_CALLBACK(on_draw_block_matrix), gui);
    gtk_box_pack_start(GTK_BOX(workspace_box), gui->drawing_block_matrix, TRUE, TRUE, 0);

    /* =====================================================================
     * 3. BOTTOM PANEL: SHA-256 AUDIT & COLORIZED KERNEL LOG STREAM
     * ===================================================================== */
    GtkWidget *bot_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_style_context_add_class(gtk_widget_get_style_context(bot_panel), "bottom-panel");
    gtk_box_pack_start(GTK_BOX(gui->page_workspace), bot_panel, FALSE, FALSE, 0);

    /* Checksum Audit Row */
    GtkWidget *sha_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(bot_panel), sha_box, FALSE, FALSE, 0);

    GtkWidget *lbl_exp_tag = gtk_label_new("EXPECTED SHA-256:");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_exp_tag), "section-header");
    gtk_box_pack_start(GTK_BOX(sha_box), lbl_exp_tag, FALSE, FALSE, 0);

    gui->lbl_expected_sha = gtk_label_new("----------------------------------------------------------------");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_expected_sha), "telemetry-val");
    gtk_box_pack_start(GTK_BOX(sha_box), gui->lbl_expected_sha, FALSE, FALSE, 0);

    GtkWidget *lbl_act_tag = gtk_label_new("ACTUAL SHA-256:");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_act_tag), "section-header");
    gtk_box_pack_start(GTK_BOX(sha_box), lbl_act_tag, FALSE, FALSE, 8);

    gui->lbl_actual_sha = gtk_label_new("----------------------------------------------------------------");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_actual_sha), "telemetry-val");
    gtk_box_pack_start(GTK_BOX(sha_box), gui->lbl_actual_sha, FALSE, FALSE, 0);

    gui->lbl_integrity_badge = gtk_label_new("[ STATUS: STANDBY // IDLE ]");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_integrity_badge), "badge-idle");
    gtk_box_pack_end(GTK_BOX(sha_box), gui->lbl_integrity_badge, FALSE, FALSE, 0);

    /* Monospace Technical Log Area with Syntax Highlighting Tags */
    GtkWidget *scrolled_log = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolled_log, -1, 130);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_log), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(bot_panel), scrolled_log, FALSE, FALSE, 0);

    gui->text_view_logs = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(gui->text_view_logs), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(gui->text_view_logs), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(gui->text_view_logs), TRUE);
    gui->text_buffer_logs = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gui->text_view_logs));
    gtk_container_add(GTK_CONTAINER(scrolled_log), gui->text_view_logs);

    /* Setup Text Tags for Syntax Highlighting */
    gui->tag_timestamp = gtk_text_buffer_create_tag(gui->text_buffer_logs, "tag_timestamp",
        "foreground", "#7E7E78", NULL);
    gui->tag_producer = gtk_text_buffer_create_tag(gui->text_buffer_logs, "tag_producer",
        "foreground", "#4A88C7", "weight", PANGO_WEIGHT_BOLD, NULL);
    gui->tag_consumer = gtk_text_buffer_create_tag(gui->text_buffer_logs, "tag_consumer",
        "foreground", "#4E9A67", "weight", PANGO_WEIGHT_BOLD, NULL);
    gui->tag_action_enqueue = gtk_text_buffer_create_tag(gui->text_buffer_logs, "tag_action_enqueue",
        "foreground", "#D4973B", NULL);
    gui->tag_action_commit = gtk_text_buffer_create_tag(gui->text_buffer_logs, "tag_action_commit",
        "foreground", "#5CB87A", "weight", PANGO_WEIGHT_BOLD, NULL);
    gui->tag_action_race = gtk_text_buffer_create_tag(gui->text_buffer_logs, "tag_action_race",
        "foreground", "#E05353", "weight", PANGO_WEIGHT_BOLD, NULL);
    gui->tag_action_sys = gtk_text_buffer_create_tag(gui->text_buffer_logs, "tag_action_sys",
        "foreground", "#A0A09A", NULL);

    /* =====================================================================
     * PAGE 2: DOWNLOAD HISTORY & AUDIT VIEW
     * ===================================================================== */
    gui->page_history = create_history_page(gui);
    gtk_stack_add_named(GTK_STACK(gui->main_stack), gui->page_history, "history");

    /* Default to Transfer Engine Workspace page */
    gtk_stack_set_visible_child_name(GTK_STACK(gui->main_stack), "workspace");

    /* Initial Announcement */
    app_log(ctx, "SYS_INIT", "WORKSTATION", "Native GTK3 Concurrency Workstation initialized. System ready.");

    /* Start High-Frequency Telemetry Timer (30 FPS / ~33ms) */
    gui->timer_id = g_timeout_add(33, on_gui_telemetry_tick, gui);

    gtk_widget_show_all(gui->main_window);
    return 0;
}
