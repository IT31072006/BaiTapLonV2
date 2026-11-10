#ifndef GUI_DASHBOARD_H
#define GUI_DASHBOARD_H

#include "common.h"
#include <gtk/gtk.h>

#define SPARKLINE_POINTS 60

/* GTK Dashboard context holding widget references */
typedef struct {
    AppContext *ctx;
    GtkWidget *main_window;

    /* Top Toolbar Controls */
    GtkWidget *entry_url;
    GtkWidget *btn_gen_32mb;
    GtkWidget *btn_gen_16mb;
    GtkWidget *spin_producers;
    GtkWidget *spin_consumers;
    GtkWidget *switch_sync_mode;
    GtkWidget *lbl_sync_mode;
    GtkWidget *btn_start;
    GtkWidget *btn_pause;
    GtkWidget *btn_reset;

    /* Left Sidebar Telemetry Labels & Cards */
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

/* Signal Callbacks */
void on_btn_start_clicked(GtkButton *button, gpointer user_data);
void on_btn_pause_clicked(GtkButton *button, gpointer user_data);
void on_btn_reset_clicked(GtkButton *button, gpointer user_data);
void on_btn_gen_file_clicked(GtkButton *button, gpointer user_data);
void on_switch_sync_toggled(GtkSwitch *widget, gboolean state, gpointer user_data);

#endif /* GUI_DASHBOARD_H */
