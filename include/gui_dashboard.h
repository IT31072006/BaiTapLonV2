#ifndef GUI_DASHBOARD_H
#define GUI_DASHBOARD_H

#include "common.h"
#include <gtk/gtk.h>

#define SPARKLINE_POINTS 60
#define MAX_HISTORY_RECORDS 128

/* Download History Record structure */
typedef struct {
    char timestamp[32];
    char filename[256];
    char target_url[512];
    char filepath[512];
    size_t file_size;
    double duration_sec;
    double speed_mbps;
    SyncMode sync_mode;
    bool is_verified;
    uint64_t race_errors;
    char sha256[65];
} DownloadHistoryRecord;

/* GTK Dashboard context holding widget references */
typedef struct {
    AppContext *ctx;
    GtkWidget *main_window;

    /* Multi-Page Navigation & Stack */
    GtkWidget *main_stack;
    GtkWidget *btn_nav_workspace;
    GtkWidget *btn_nav_history;
    GtkWidget *lbl_nav_hist_badge;
    GtkWidget *page_workspace;
    GtkWidget *page_history;

    /* Top Toolbar Controls (Workspace Page) */
    GtkWidget *entry_url;
    GtkWidget *lbl_source_badge;
    GtkWidget *btn_test_offline;
    GtkWidget *btn_auto_tune;
    GtkWidget *spin_producers;
    GtkWidget *spin_consumers;
    GtkWidget *switch_sync_mode;
    GtkWidget *lbl_sync_mode;
    GtkWidget *btn_start;
    GtkWidget *btn_pause;
    GtkWidget *btn_reset;

    /* Left Sidebar Telemetry Labels & Cards */
    GtkWidget *lbl_target_source;
    GtkWidget *lbl_engine_state;
    GtkWidget *lbl_rate;
    GtkWidget *lbl_sem_empty;
    GtkWidget *lbl_sem_full;
    GtkWidget *lbl_contentions;
    GtkWidget *lbl_race_errors;
    GtkWidget *lbl_bytes_downloaded;
    GtkWidget *lbl_bytes_written;
    GtkWidget *progress_bar;
    GtkWidget *drawing_sidebar_sparkline; /* Live Real-time Throughput Graph */

    /* Center Cairo Canvases */
    GtkWidget *drawing_ram_buffer;
    GtkWidget *drawing_block_matrix;

    /* Bottom Status & Log Widgets */
    GtkWidget *lbl_expected_sha;
    GtkWidget *lbl_actual_sha;
    GtkWidget *lbl_integrity_badge;
    GtkWidget *text_view_logs;
    GtkTextBuffer *text_buffer_logs;
    int last_rendered_log_count;

    /* Page 2: Download History Widgets & Records */
    DownloadHistoryRecord history_items[MAX_HISTORY_RECORDS];
    int history_count;
    GtkWidget *lbl_hist_total;
    GtkWidget *lbl_hist_verified;
    GtkWidget *lbl_hist_corrupted;
    GtkWidget *lbl_hist_volume;
    GtkListStore *history_store;
    GtkWidget *tree_history;

    /* Speed History for Live Sparkline */
    double speed_history[SPARKLINE_POINTS];
    int speed_history_idx;
    double max_speed_recorded;

    /* Text Tags for Syntax Colorized Console */
    GtkTextTag *tag_timestamp;
    GtkTextTag *tag_producer;
    GtkTextTag *tag_consumer;
    GtkTextTag *tag_action_enqueue;
    GtkTextTag *tag_action_commit;
    GtkTextTag *tag_action_race;
    GtkTextTag *tag_action_sys;

    /* Execution Control */
    pthread_t engine_worker_thread;
    bool worker_running;
    bool auto_tune_enabled;
    guint timer_id;
} GuiDashboard;

/* Initializes and constructs the GTK3 Native Industrial Dashboard */
int gui_dashboard_create(GuiDashboard *gui, AppContext *ctx, int argc, char **argv);

/* Cairo 2D drawing routines */
gboolean on_draw_ram_buffer(GtkWidget *widget, cairo_t *cr, gpointer user_data);
gboolean on_draw_block_matrix(GtkWidget *widget, cairo_t *cr, gpointer user_data);
gboolean on_draw_sidebar_sparkline(GtkWidget *widget, cairo_t *cr, gpointer user_data);

/* Periodic telemetry tick (30 FPS / 33ms) */
gboolean on_gui_telemetry_tick(gpointer user_data);

/* Signal Callbacks & Automation */
void on_btn_start_clicked(GtkButton *button, gpointer user_data);
void on_btn_pause_clicked(GtkButton *button, gpointer user_data);
void on_btn_reset_clicked(GtkButton *button, gpointer user_data);
void on_btn_test_offline_clicked(GtkButton *button, gpointer user_data);
void on_btn_auto_tune_toggled(GtkToggleButton *button, gpointer user_data);
void on_switch_sync_toggled(GtkSwitch *widget, gboolean state, gpointer user_data);
void on_entry_url_changed(GtkEditable *editable, gpointer user_data);
void auto_optimize_concurrency(GuiDashboard *gui);

/* Page Navigation & History Callbacks */
void on_nav_workspace_clicked(GtkButton *button, gpointer user_data);
void on_nav_history_clicked(GtkButton *button, gpointer user_data);
void on_btn_open_storage_clicked(GtkButton *button, gpointer user_data);
void on_btn_clear_history_clicked(GtkButton *button, gpointer user_data);
void on_btn_copy_sha_clicked(GtkButton *button, gpointer user_data);
void gui_add_history_record(GuiDashboard *gui, const DownloadHistoryRecord *rec);

#endif /* GUI_DASHBOARD_H */
