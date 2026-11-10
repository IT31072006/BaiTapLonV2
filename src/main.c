#include "common.h"
#include "network_producer.h"
#include "storage_consumer.h"
#include "gui_dashboard.h"
#if __has_include("curl/curl.h")
#include "curl/curl.h"
#else
#include <curl/curl.h>
#endif

int main(int argc, char **argv) {
    /* Initialize libcurl runtime engine globally */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Allocate and initialize central synchronization context */
    AppContext ctx;
    app_context_init(&ctx);

    /* Allocate GUI Dashboard controller */
    GuiDashboard gui;
    memset(&gui, 0, sizeof(GuiDashboard));

    /* Build Native Industrial Workstation UI and enter GTK Event Loop */
    if (gui_dashboard_create(&gui, &ctx, argc, argv) != 0) {
        fprintf(stderr, "Failed to initialize native workstation interface.\n");
        curl_global_cleanup();
        return 1;
    }

    /* Start GTK3 Main Event Dispatcher (Blocks until window closed) */
    gtk_main();

    /* Clean Shutdown Pipeline */
    if (gui.worker_running) {
        ctx.stop_requested = true;
        pthread_cond_broadcast(&ctx.pause_cond);
        pthread_join(gui.engine_worker_thread, NULL);
    }

    stop_local_http_server(&ctx);
    app_context_destroy(&ctx);
    curl_global_cleanup();

    return 0;
}

