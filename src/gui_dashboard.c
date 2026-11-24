#include "gui_dashboard.h"
#include "network_producer.h"
#include "storage_consumer.h"
#include <math.h>
#include <pango/pangocairo.h>

/* =========================================================================
 * 1. LIGHT THEME CSS: MODERN ATLASSIAN / JIRA SAAS (TRẮNG - XANH HOÀNG GIA)
 * ========================================================================= */
static const char *WORKSTATION_CSS_LIGHT = 
    "* {\n"
    "    font-family: 'Liberation Sans', 'DejaVu Sans', 'Noto Sans', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Ubuntu, Arial, sans-serif;\n"
    "    font-size: 13px;\n"
    "    color: #0F172A;\n"
    "}\n"
    "window, .main-window {\n"
    "    background-color: #F8FAFC;\n"
    "}\n"
    "button, button:hover, button:active, button:checked, button:focus, button:backdrop {\n"
    "    background-image: none;\n"
    "    box-shadow: none;\n"
    "    text-shadow: none;\n"
    "    outline: none;\n"
    "    border-style: solid;\n"
    "}\n"
    "button label {\n"
    "    color: inherit;\n"
    "}\n"
    "/* Left Navigation Sidebar (White & Royal Blue SaaS Style) */\n"
    ".nav-sidebar {\n"
    "    background-color: #F0F4FA;\n"
    "    border-right: 1px solid #D0DFEF;\n"
    "    padding: 16px 12px;\n"
    "}\n"
    ".brand-badge {\n"
    "    background-color: #0C66E4;\n"
    "    color: #FFFFFF;\n"
    "    font-size: 14px;\n"
    "    font-weight: 800;\n"
    "    border-radius: 7px;\n"
    "    padding: 7px 11px;\n"
    "}\n"
    ".nav-brand-title {\n"
    "    font-size: 15.5px;\n"
    "    font-weight: 800;\n"
    "    color: #0F172A;\n"
    "}\n"
    ".nav-brand-subtitle {\n"
    "    font-size: 11px;\n"
    "    font-weight: 700;\n"
    "    color: #0C66E4;\n"
    "}\n"
    ".nav-section-label {\n"
    "    font-size: 11.5px;\n"
    "    font-weight: 700;\n"
    "    color: #334155;\n"
    "    margin-top: 14px;\n"
    "    margin-bottom: 6px;\n"
    "    padding-left: 6px;\n"
    "}\n"
    "button.nav-item {\n"
    "    background-image: none;\n"
    "    background-color: transparent;\n"
    "    border: 1px solid transparent;\n"
    "    border-radius: 7px;\n"
    "    padding: 8px 12px;\n"
    "}\n"
    "button.nav-item label {\n"
    "    color: #1E293B;\n"
    "    font-size: 13px;\n"
    "    font-weight: 600;\n"
    "}\n"
    "button.nav-item:hover {\n"
    "    background-image: none;\n"
    "    background-color: #E2ECF8;\n"
    "    border: 1px solid #CBD5E1;\n"
    "}\n"
    "button.nav-item:hover label {\n"
    "    color: #0C66E4;\n"
    "}\n"
    "button.nav-item-active {\n"
    "    background-image: none;\n"
    "    background-color: #0C66E4;\n"
    "    border: 1px solid #0055CC;\n"
    "    border-radius: 7px;\n"
    "}\n"
    "button.nav-item-active label {\n"
    "    color: #FFFFFF;\n"
    "    font-weight: 700;\n"
    "}\n"
    ".nav-badge-count {\n"
    "    background-color: #E2ECF8;\n"
    "    color: #0C66E4;\n"
    "    border: 1px solid #B3D4FF;\n"
    "    border-radius: 9999px;\n"
    "    padding: 2px 8px;\n"
    "    font-size: 11px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-weight: 700;\n"
    "}\n"
    "button.nav-item-active .nav-badge-count {\n"
    "    background-color: rgba(255, 255, 255, 0.25);\n"
    "    color: #FFFFFF;\n"
    "    border: 1px solid rgba(255, 255, 255, 0.35);\n"
    "}\n"
    "button.nav-btn-action {\n"
    "    background-image: none;\n"
    "    background-color: #FFFFFF;\n"
    "    border: 1px solid #0C66E4;\n"
    "    border-radius: 7px;\n"
    "    padding: 7px 12px;\n"
    "}\n"
    "button.nav-btn-action label {\n"
    "    color: #0C66E4;\n"
    "    font-weight: 700;\n"
    "    font-size: 12px;\n"
    "}\n"
    "button.nav-btn-action:hover {\n"
    "    background-image: none;\n"
    "    background-color: #0C66E4;\n"
    "    border-color: #0055CC;\n"
    "}\n"
    "button.nav-btn-action:hover label {\n"
    "    color: #FFFFFF;\n"
    "}\n"
    "button.btn-theme-toggle {\n"
    "    background-image: none;\n"
    "    background-color: #FFFFFF;\n"
    "    border: 1px solid #CBD5E1;\n"
    "    border-radius: 7px;\n"
    "    padding: 6px 12px;\n"
    "}\n"
    "button.btn-theme-toggle label {\n"
    "    color: #334155;\n"
    "    font-weight: 600;\n"
    "    font-size: 12px;\n"
    "}\n"
    "button.btn-theme-toggle:hover {\n"
    "    background-image: none;\n"
    "    background-color: #F1F5F9;\n"
    "    border-color: #94A3B8;\n"
    "}\n"
    "button.btn-theme-toggle:hover label {\n"
    "    color: #0F172A;\n"
    "}\n"
    ".nav-footer-box {\n"
    "    background-color: #FFFFFF;\n"
    "    border: 1px solid #D0DFEF;\n"
    "    border-radius: 7px;\n"
    "    padding: 8px 10px;\n"
    "    margin-top: 8px;\n"
    "}\n"
    ".nav-footer-text {\n"
    "    font-size: 11px;\n"
    "    color: #334155;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-weight: 600;\n"
    "}\n"
    "/* Panels */\n"
    ".top-toolbar {\n"
    "    background-color: #FFFFFF;\n"
    "    border-bottom: 1px solid #E2E8F0;\n"
    "    padding: 8px 14px;\n"
    "}\n"
    ".sidebar-panel {\n"
    "    background-color: #FFFFFF;\n"
    "    border-right: 1px solid #E2E8F0;\n"
    "    padding: 10px 12px;\n"
    "}\n"
    ".workspace-panel {\n"
    "    background-color: #F8FAFC;\n"
    "    padding: 10px 14px;\n"
    "}\n"
    ".bottom-panel {\n"
    "    background-color: #FFFFFF;\n"
    "    border-top: 1px solid #E2E8F0;\n"
    "    padding: 8px 14px;\n"
    "}\n"
    "/* Telemetry Cards */\n"
    ".telemetry-card {\n"
    "    background-color: #FFFFFF;\n"
    "    border: 1px solid #E2E8F0;\n"
    "    border-radius: 6px;\n"
    "    padding: 6px 10px;\n"
    "    margin-bottom: 5px;\n"
    "}\n"
    ".section-header {\n"
    "    font-size: 12.5px;\n"
    "    font-weight: 700;\n"
    "    color: #0F172A;\n"
    "}\n"
    ".telemetry-key {\n"
    "    font-size: 12px;\n"
    "    font-weight: 600;\n"
    "    color: #334155;\n"
    "}\n"
    ".telemetry-val {\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 14px;\n"
    "    font-weight: 700;\n"
    "    color: #0F172A;\n"
    "    font-feature-settings: 'tnum';\n"
    "}\n"
    ".telemetry-val-large {\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 20px;\n"
    "    font-weight: 800;\n"
    "    color: #0C66E4;\n"
    "    font-feature-settings: 'tnum';\n"
    "}\n"
    ".telemetry-val-alert {\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 14.5px;\n"
    "    font-weight: 700;\n"
    "    color: #DC2626;\n"
    "    font-feature-settings: 'tnum';\n"
    "}\n"
    ".telemetry-val-success {\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 14px;\n"
    "    font-weight: 700;\n"
    "    color: #16A34A;\n"
    "}\n"
    "/* Action Buttons */\n"
    "button.btn-primary {\n"
    "    background-image: none;\n"
    "    background-color: #0C66E4;\n"
    "    border: 1px solid #0055CC;\n"
    "    border-radius: 6px;\n"
    "    padding: 6px 14px;\n"
    "}\n"
    "button.btn-primary label {\n"
    "    color: #FFFFFF;\n"
    "    font-weight: 700;\n"
    "    font-size: 12.5px;\n"
    "}\n"
    "button.btn-primary:hover {\n"
    "    background-image: none;\n"
    "    background-color: #0055CC;\n"
    "    border-color: #0047AB;\n"
    "}\n"
    "button.btn-secondary {\n"
    "    background-image: none;\n"
    "    background-color: #FFFFFF;\n"
    "    border: 1px solid #CBD5E1;\n"
    "    border-radius: 6px;\n"
    "    padding: 6px 12px;\n"
    "}\n"
    "button.btn-secondary label {\n"
    "    color: #1E293B;\n"
    "    font-weight: 700;\n"
    "    font-size: 12.5px;\n"
    "}\n"
    "button.btn-secondary:hover {\n"
    "    background-image: none;\n"
    "    background-color: #F8FAFC;\n"
    "    border-color: #94A3B8;\n"
    "}\n"
    "button.btn-secondary:hover label {\n"
    "    color: #0F172A;\n"
    "}\n"
    "button.btn-pause {\n"
    "    background-image: none;\n"
    "    background-color: #FFFFFF;\n"
    "    border: 1px solid #CBD5E1;\n"
    "    border-radius: 6px;\n"
    "    padding: 6px 12px;\n"
    "}\n"
    "button.btn-pause label {\n"
    "    color: #334155;\n"
    "    font-weight: 600;\n"
    "    font-size: 12.5px;\n"
    "}\n"
    "button.btn-pause:hover {\n"
    "    background-image: none;\n"
    "    background-color: #FEF3C7;\n"
    "    border-color: #FDE68A;\n"
    "}\n"
    "button.btn-pause:hover label {\n"
    "    color: #B45309;\n"
    "}\n"
    "button.btn-reset {\n"
    "    background-image: none;\n"
    "    background-color: #FFFFFF;\n"
    "    border: 1px solid #CBD5E1;\n"
    "    border-radius: 6px;\n"
    "    padding: 6px 12px;\n"
    "}\n"
    "button.btn-reset label {\n"
    "    color: #334155;\n"
    "    font-weight: 600;\n"
    "    font-size: 12.5px;\n"
    "}\n"
    "button.btn-reset:hover {\n"
    "    background-image: none;\n"
    "    background-color: #FEE2E2;\n"
    "    border-color: #FECACA;\n"
    "}\n"
    "button.btn-reset:hover label {\n"
    "    color: #DC2626;\n"
    "}\n"
    "button.btn-steel {\n"
    "    background-image: none;\n"
    "    background-color: #EFF6FF;\n"
    "    border: 1px solid #B3D4FF;\n"
    "    border-radius: 6px;\n"
    "    padding: 6px 12px;\n"
    "}\n"
    "button.btn-steel label {\n"
    "    color: #0C66E4;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    "button.btn-steel:hover {\n"
    "    background-image: none;\n"
    "    background-color: #DBEAFE;\n"
    "    border-color: #93C5FD;\n"
    "}\n"
    "button.btn-steel:hover label {\n"
    "    color: #0055CC;\n"
    "}\n"
    "button.btn-active {\n"
    "    background-image: none;\n"
    "    background-color: #0C66E4;\n"
    "    border: 1px solid #0055CC;\n"
    "}\n"
    "button.btn-active label {\n"
    "    color: #FFFFFF;\n"
    "    font-weight: 700;\n"
    "}\n"
    "button.btn-auto-on {\n"
    "    background-image: none;\n"
    "    background-color: #EFF6FF;\n"
    "    border: 1px solid #93C5FD;\n"
    "    border-radius: 6px;\n"
    "    padding: 5px 10px;\n"
    "}\n"
    "button.btn-auto-on label {\n"
    "    color: #0C66E4;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    "button.btn-auto-off {\n"
    "    background-image: none;\n"
    "    background-color: #F8FAFC;\n"
    "    border: 1px solid #CBD5E1;\n"
    "    border-radius: 6px;\n"
    "    padding: 5px 10px;\n"
    "}\n"
    "button.btn-auto-off label {\n"
    "    color: #475569;\n"
    "    font-weight: 600;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    ".badge-dataset {\n"
    "    background-color: #EFF6FF;\n"
    "    color: #0C66E4;\n"
    "    border: 1px solid #B3D4FF;\n"
    "    border-radius: 4px;\n"
    "    padding: 4px 9px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    ".badge-dataset-net {\n"
    "    background-color: #FFFBEB;\n"
    "    color: #D97706;\n"
    "    border: 1px solid #FDE68A;\n"
    "    border-radius: 4px;\n"
    "    padding: 4px 9px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    "entry {\n"
    "    background-image: none;\n"
    "    background-color: #FFFFFF;\n"
    "    color: #0F172A;\n"
    "    border: 1px solid #CBD5E1;\n"
    "    border-radius: 6px;\n"
    "    padding: 6px 10px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 12.5px;\n"
    "}\n"
    "entry:focus {\n"
    "    border-color: #0C66E4;\n"
    "}\n"
    "spinbutton {\n"
    "    background-image: none;\n"
    "    background-color: #FFFFFF;\n"
    "    border: 1px solid #CBD5E1;\n"
    "    border-radius: 6px;\n"
    "    padding: 3px 5px;\n"
    "}\n"
    "spinbutton entry {\n"
    "    border: none;\n"
    "    background-color: transparent;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-weight: 700;\n"
    "    font-size: 13px;\n"
    "    color: #0C66E4;\n"
    "}\n"
    "spinbutton button {\n"
    "    background-image: none;\n"
    "    background-color: #F1F5F9;\n"
    "    border: none;\n"
    "    color: #334155;\n"
    "}\n"
    "spinbutton button:hover {\n"
    "    background-image: none;\n"
    "    background-color: #E2E8F0;\n"
    "    color: #0F172A;\n"
    "}\n"
    "switch {\n"
    "    background-image: none;\n"
    "    background-color: #CBD5E1;\n"
    "    border-radius: 12px;\n"
    "}\n"
    "switch:checked {\n"
    "    background-image: none;\n"
    "    background-color: #16A34A;\n"
    "}\n"
    "switch slider {\n"
    "    background-image: none;\n"
    "    background-color: #FFFFFF;\n"
    "    border-radius: 10px;\n"
    "    min-width: 18px;\n"
    "    min-height: 18px;\n"
    "    margin: 1px;\n"
    "}\n"
    "textview text {\n"
    "    background-color: #F8FAFC;\n"
    "    color: #0F172A;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 12px;\n"
    "}\n"
    "progressbar trough {\n"
    "    background-color: #E2E8F0;\n"
    "    border: 1px solid #CBD5E1;\n"
    "    border-radius: 4px;\n"
    "    min-height: 8px;\n"
    "}\n"
    "progressbar progress {\n"
    "    background-image: none;\n"
    "    background-color: #0C66E4;\n"
    "    border-radius: 4px;\n"
    "}\n"
    ".badge-idle {\n"
    "    background-color: #F1F5F9;\n"
    "    color: #475569;\n"
    "    border: 1px solid #CBD5E1;\n"
    "    border-radius: 4px;\n"
    "    padding: 4px 10px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    ".badge-running {\n"
    "    background-color: #EFF6FF;\n"
    "    color: #0C66E4;\n"
    "    border: 1px solid #B3D4FF;\n"
    "    border-radius: 4px;\n"
    "    padding: 4px 10px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    ".badge-verified {\n"
    "    background-color: #F0FDF4;\n"
    "    color: #16A34A;\n"
    "    border: 1px solid #BBF7D0;\n"
    "    border-radius: 4px;\n"
    "    padding: 4px 10px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    ".badge-corrupted {\n"
    "    background-color: #FEF2F2;\n"
    "    color: #DC2626;\n"
    "    border: 1px solid #FECACA;\n"
    "    border-radius: 4px;\n"
    "    padding: 4px 10px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    ".mode-synced {\n"
    "    color: #16A34A;\n"
    "    font-weight: 700;\n"
    "    font-size: 12px;\n"
    "}\n"
    ".mode-unsynced {\n"
    "    color: #DC2626;\n"
    "    font-weight: 700;\n"
    "    font-size: 12px;\n"
    "}\n"
    "separator {\n"
    "    background-color: #E2E8F0;\n"
    "    min-width: 1px;\n"
    "    min-height: 1px;\n"
    "}\n"
    "/* Pages */\n"
    ".history-page, .benchmark-page {\n"
    "    background-color: #F8FAFC;\n"
    "    padding: 16px 20px;\n"
    "}\n"
    ".history-header-title, .bm-header-title {\n"
    "    font-size: 17px;\n"
    "    font-weight: 800;\n"
    "    letter-spacing: -0.01em;\n"
    "    color: #0F172A;\n"
    "}\n"
    ".history-header-desc, .bm-header-desc {\n"
    "    font-size: 12.5px;\n"
    "    color: #334155;\n"
    "    margin-bottom: 12px;\n"
    "}\n"
    ".history-card-metric, .bm-card {\n"
    "    background-color: #FFFFFF;\n"
    "    border: 1px solid #E2E8F0;\n"
    "    border-radius: 8px;\n"
    "    padding: 12px 16px;\n"
    "}\n"
    ".bm-summary-box {\n"
    "    background-color: #FFFFFF;\n"
    "    border: 1px solid #E2E8F0;\n"
    "    border-radius: 8px;\n"
    "    padding: 12px 16px;\n"
    "}\n"
    ".history-metric-title {\n"
    "    font-size: 11.5px;\n"
    "    font-weight: 700;\n"
    "    color: #475569;\n"
    "    letter-spacing: 0.03em;\n"
    "}\n"
    ".history-metric-val {\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 22px;\n"
    "    font-weight: 800;\n"
    "    color: #0F172A;\n"
    "}\n"
    ".history-metric-val-ok {\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 22px;\n"
    "    font-weight: 800;\n"
    "    color: #16A34A;\n"
    "}\n"
    ".history-metric-val-err {\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 22px;\n"
    "    font-weight: 800;\n"
    "    color: #DC2626;\n"
    "}\n"
    "treeview.history-tree {\n"
    "    background-color: #FFFFFF;\n"
    "    color: #0F172A;\n"
    "    border: 1px solid #E2E8F0;\n"
    "    border-radius: 6px;\n"
    "    font-size: 12.5px;\n"
    "}\n"
    "treeview.history-tree:selected {\n"
    "    background-color: #EFF6FF;\n"
    "    color: #0C66E4;\n"
    "}\n"
    "treeview.history-tree header button {\n"
    "    background-image: none;\n"
    "    background-color: #F1F5F9;\n"
    "    color: #0F172A;\n"
    "    font-weight: 700;\n"
    "    font-size: 12px;\n"
    "    border: 1px solid #E2E8F0;\n"
    "    padding: 7px 10px;\n"
    "}\n"
    "treeview.history-tree header button label {\n"
    "    color: #0F172A;\n"
    "    font-weight: 700;\n"
    "}\n";

static const char *WORKSTATION_CSS_DARK = 
    "* {\n"
    "    font-family: 'Liberation Sans', 'DejaVu Sans', 'Noto Sans', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Ubuntu, Arial, sans-serif;\n"
    "    font-size: 13px;\n"
    "    color: #F8FAFC;\n"
    "}\n"
    "window, .main-window {\n"
    "    background-color: #141413;\n"
    "}\n"
    "button, button:hover, button:active, button:checked, button:focus, button:backdrop {\n"
    "    background-image: none;\n"
    "    box-shadow: none;\n"
    "    text-shadow: none;\n"
    "    outline: none;\n"
    "    border-style: solid;\n"
    "}\n"
    "button label {\n"
    "    color: inherit;\n"
    "}\n"
    ".nav-sidebar {\n"
    "    background-color: #1C1C1A;\n"
    "    border-right: 1px solid #2C2C29;\n"
    "    padding: 16px 12px;\n"
    "}\n"
    ".brand-badge {\n"
    "    background-color: #1F6FEB;\n"
    "    color: #FFFFFF;\n"
    "    font-size: 14px;\n"
    "    font-weight: 800;\n"
    "    border-radius: 7px;\n"
    "    padding: 7px 11px;\n"
    "}\n"
    ".nav-brand-title {\n"
    "    font-size: 15.5px;\n"
    "    font-weight: 800;\n"
    "    color: #F8FAFC;\n"
    "}\n"
    ".nav-brand-subtitle {\n"
    "    font-size: 11px;\n"
    "    font-weight: 700;\n"
    "    color: #58A6FF;\n"
    "}\n"
    ".nav-section-label {\n"
    "    font-size: 11.5px;\n"
    "    font-weight: 700;\n"
    "    color: #94A3B8;\n"
    "    margin-top: 14px;\n"
    "    margin-bottom: 6px;\n"
    "    padding-left: 6px;\n"
    "}\n"
    "button.nav-item {\n"
    "    background-image: none;\n"
    "    background-color: transparent;\n"
    "    border: 1px solid transparent;\n"
    "    border-radius: 7px;\n"
    "    padding: 8px 12px;\n"
    "}\n"
    "button.nav-item label {\n"
    "    color: #CBD5E1;\n"
    "    font-size: 13px;\n"
    "    font-weight: 600;\n"
    "}\n"
    "button.nav-item:hover {\n"
    "    background-image: none;\n"
    "    background-color: #262624;\n"
    "    border: 1px solid #30363D;\n"
    "}\n"
    "button.nav-item:hover label {\n"
    "    color: #58A6FF;\n"
    "}\n"
    "button.nav-item-active {\n"
    "    background-image: none;\n"
    "    background-color: #1F6FEB;\n"
    "    border: 1px solid #388BFD;\n"
    "    border-radius: 7px;\n"
    "}\n"
    "button.nav-item-active label {\n"
    "    color: #FFFFFF;\n"
    "    font-weight: 700;\n"
    "}\n"
    ".nav-badge-count {\n"
    "    background-color: #21262D;\n"
    "    color: #58A6FF;\n"
    "    border: 1px solid #30363D;\n"
    "    border-radius: 9999px;\n"
    "    padding: 2px 8px;\n"
    "    font-size: 11px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-weight: 700;\n"
    "}\n"
    "button.nav-item-active .nav-badge-count {\n"
    "    background-color: rgba(255, 255, 255, 0.25);\n"
    "    color: #FFFFFF;\n"
    "    border: 1px solid rgba(255, 255, 255, 0.35);\n"
    "}\n"
    "button.nav-btn-action {\n"
    "    background-image: none;\n"
    "    background-color: #21262D;\n"
    "    border: 1px solid #388BFD;\n"
    "    border-radius: 7px;\n"
    "    padding: 7px 12px;\n"
    "}\n"
    "button.nav-btn-action label {\n"
    "    color: #58A6FF;\n"
    "    font-weight: 700;\n"
    "    font-size: 12px;\n"
    "}\n"
    "button.nav-btn-action:hover {\n"
    "    background-image: none;\n"
    "    background-color: #1F6FEB;\n"
    "    border-color: #388BFD;\n"
    "}\n"
    "button.nav-btn-action:hover label {\n"
    "    color: #FFFFFF;\n"
    "}\n"
    "button.btn-theme-toggle {\n"
    "    background-image: none;\n"
    "    background-color: #21262D;\n"
    "    border: 1px solid #30363D;\n"
    "    border-radius: 7px;\n"
    "    padding: 6px 12px;\n"
    "}\n"
    "button.btn-theme-toggle label {\n"
    "    color: #CBD5E1;\n"
    "    font-weight: 600;\n"
    "    font-size: 12px;\n"
    "}\n"
    "button.btn-theme-toggle:hover {\n"
    "    background-image: none;\n"
    "    background-color: #30363D;\n"
    "    border-color: #58A6FF;\n"
    "}\n"
    "button.btn-theme-toggle:hover label {\n"
    "    color: #F8FAFC;\n"
    "}\n"
    ".nav-footer-box {\n"
    "    background-color: #1C1C1A;\n"
    "    border: 1px solid #2C2C29;\n"
    "    border-radius: 7px;\n"
    "    padding: 8px 10px;\n"
    "    margin-top: 8px;\n"
    "}\n"
    ".nav-footer-text {\n"
    "    font-size: 11px;\n"
    "    color: #94A3B8;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-weight: 600;\n"
    "}\n"
    "/* Panels */\n"
    ".top-toolbar {\n"
    "    background-color: #1C1C1A;\n"
    "    border-bottom: 1px solid #2C2C29;\n"
    "    padding: 8px 14px;\n"
    "}\n"
    ".sidebar-panel {\n"
    "    background-color: #1C1C1A;\n"
    "    border-right: 1px solid #2C2C29;\n"
    "    padding: 10px 12px;\n"
    "}\n"
    ".workspace-panel {\n"
    "    background-color: #141413;\n"
    "    padding: 10px 14px;\n"
    "}\n"
    ".bottom-panel {\n"
    "    background-color: #1C1C1A;\n"
    "    border-top: 1px solid #2C2C29;\n"
    "    padding: 8px 14px;\n"
    "}\n"
    "/* Telemetry Cards */\n"
    ".telemetry-card {\n"
    "    background-color: #1C1C1A;\n"
    "    border: 1px solid #2C2C29;\n"
    "    border-radius: 6px;\n"
    "    padding: 6px 10px;\n"
    "    margin-bottom: 5px;\n"
    "}\n"
    ".section-header {\n"
    "    font-size: 12.5px;\n"
    "    font-weight: 700;\n"
    "    color: #F8FAFC;\n"
    "}\n"
    ".telemetry-key {\n"
    "    font-size: 12px;\n"
    "    font-weight: 600;\n"
    "    color: #94A3B8;\n"
    "}\n"
    ".telemetry-val {\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 14px;\n"
    "    font-weight: 700;\n"
    "    color: #F8FAFC;\n"
    "    font-feature-settings: 'tnum';\n"
    "}\n"
    ".telemetry-val-large {\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 20px;\n"
    "    font-weight: 800;\n"
    "    color: #58A6FF;\n"
    "    font-feature-settings: 'tnum';\n"
    "}\n"
    ".telemetry-val-alert {\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 14.5px;\n"
    "    font-weight: 700;\n"
    "    color: #F85149;\n"
    "    font-feature-settings: 'tnum';\n"
    "}\n"
    ".telemetry-val-success {\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 14px;\n"
    "    font-weight: 700;\n"
    "    color: #3FB950;\n"
    "}\n"
    "/* Action Buttons */\n"
    "button.btn-primary {\n"
    "    background-image: none;\n"
    "    background-color: #1F6FEB;\n"
    "    border: 1px solid #388BFD;\n"
    "    border-radius: 6px;\n"
    "    padding: 6px 14px;\n"
    "}\n"
    "button.btn-primary label {\n"
    "    color: #FFFFFF;\n"
    "    font-weight: 700;\n"
    "    font-size: 12.5px;\n"
    "}\n"
    "button.btn-primary:hover {\n"
    "    background-image: none;\n"
    "    background-color: #388BFD;\n"
    "    border-color: #58A6FF;\n"
    "}\n"
    "button.btn-secondary {\n"
    "    background-image: none;\n"
    "    background-color: #21262D;\n"
    "    border: 1px solid #30363D;\n"
    "    border-radius: 6px;\n"
    "    padding: 6px 12px;\n"
    "}\n"
    "button.btn-secondary label {\n"
    "    color: #C9D1D9;\n"
    "    font-weight: 700;\n"
    "    font-size: 12.5px;\n"
    "}\n"
    "button.btn-secondary:hover {\n"
    "    background-image: none;\n"
    "    background-color: #30363D;\n"
    "    border-color: #8B949E;\n"
    "}\n"
    "button.btn-secondary:hover label {\n"
    "    color: #F8FAFC;\n"
    "}\n"
    "button.btn-pause {\n"
    "    background-image: none;\n"
    "    background-color: #21262D;\n"
    "    border: 1px solid #30363D;\n"
    "    border-radius: 6px;\n"
    "    padding: 6px 12px;\n"
    "}\n"
    "button.btn-pause label {\n"
    "    color: #C9D1D9;\n"
    "    font-weight: 600;\n"
    "    font-size: 12.5px;\n"
    "}\n"
    "button.btn-pause:hover {\n"
    "    background-image: none;\n"
    "    background-color: #342508;\n"
    "    border-color: #D29922;\n"
    "}\n"
    "button.btn-pause:hover label {\n"
    "    color: #F0883E;\n"
    "}\n"
    "button.btn-reset {\n"
    "    background-image: none;\n"
    "    background-color: #21262D;\n"
    "    border: 1px solid #30363D;\n"
    "    border-radius: 6px;\n"
    "    padding: 6px 12px;\n"
    "}\n"
    "button.btn-reset label {\n"
    "    color: #C9D1D9;\n"
    "    font-weight: 600;\n"
    "    font-size: 12.5px;\n"
    "}\n"
    "button.btn-reset:hover {\n"
    "    background-image: none;\n"
    "    background-color: #3C1E1E;\n"
    "    border-color: #F85149;\n"
    "}\n"
    "button.btn-reset:hover label {\n"
    "    color: #FF7B72;\n"
    "}\n"
    "button.btn-steel {\n"
    "    background-image: none;\n"
    "    background-color: #161B22;\n"
    "    border: 1px solid #30363D;\n"
    "    border-radius: 6px;\n"
    "    padding: 6px 12px;\n"
    "}\n"
    "button.btn-steel label {\n"
    "    color: #58A6FF;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    "button.btn-steel:hover {\n"
    "    background-image: none;\n"
    "    background-color: #21262D;\n"
    "    border-color: #58A6FF;\n"
    "}\n"
    "button.btn-steel:hover label {\n"
    "    color: #79C0FF;\n"
    "}\n"
    "button.btn-active {\n"
    "    background-image: none;\n"
    "    background-color: #1F6FEB;\n"
    "    border: 1px solid #388BFD;\n"
    "}\n"
    "button.btn-active label {\n"
    "    color: #FFFFFF;\n"
    "    font-weight: 700;\n"
    "}\n"
    "button.btn-auto-on {\n"
    "    background-image: none;\n"
    "    background-color: #1C2D42;\n"
    "    border: 1px solid #388BFD;\n"
    "    border-radius: 6px;\n"
    "    padding: 5px 10px;\n"
    "}\n"
    "button.btn-auto-on label {\n"
    "    color: #58A6FF;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    "button.btn-auto-off {\n"
    "    background-image: none;\n"
    "    background-color: #21262D;\n"
    "    border: 1px solid #30363D;\n"
    "    border-radius: 6px;\n"
    "    padding: 5px 10px;\n"
    "}\n"
    "button.btn-auto-off label {\n"
    "    color: #8B949E;\n"
    "    font-weight: 600;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    ".badge-dataset {\n"
    "    background-color: #161B22;\n"
    "    color: #58A6FF;\n"
    "    border: 1px solid #30363D;\n"
    "    border-radius: 4px;\n"
    "    padding: 4px 9px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    ".badge-dataset-net {\n"
    "    background-color: #2D2200;\n"
    "    color: #D29922;\n"
    "    border: 1px solid #6E4E00;\n"
    "    border-radius: 4px;\n"
    "    padding: 4px 9px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    "entry {\n"
    "    background-image: none;\n"
    "    background-color: #161B22;\n"
    "    color: #F8FAFC;\n"
    "    border: 1px solid #30363D;\n"
    "    border-radius: 6px;\n"
    "    padding: 6px 10px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 12.5px;\n"
    "}\n"
    "entry:focus {\n"
    "    border-color: #58A6FF;\n"
    "}\n"
    "spinbutton {\n"
    "    background-image: none;\n"
    "    background-color: #161B22;\n"
    "    border: 1px solid #30363D;\n"
    "    border-radius: 6px;\n"
    "    padding: 3px 5px;\n"
    "}\n"
    "spinbutton entry {\n"
    "    border: none;\n"
    "    background-color: transparent;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-weight: 700;\n"
    "    font-size: 13px;\n"
    "    color: #58A6FF;\n"
    "}\n"
    "spinbutton button {\n"
    "    background-image: none;\n"
    "    background-color: #21262D;\n"
    "    border: none;\n"
    "    color: #C9D1D9;\n"
    "}\n"
    "spinbutton button:hover {\n"
    "    background-image: none;\n"
    "    background-color: #30363D;\n"
    "    color: #F8FAFC;\n"
    "}\n"
    "switch {\n"
    "    background-image: none;\n"
    "    background-color: #30363D;\n"
    "    border-radius: 12px;\n"
    "}\n"
    "switch:checked {\n"
    "    background-image: none;\n"
    "    background-color: #238636;\n"
    "}\n"
    "switch slider {\n"
    "    background-image: none;\n"
    "    background-color: #F0F6FC;\n"
    "    border-radius: 10px;\n"
    "    min-width: 18px;\n"
    "    min-height: 18px;\n"
    "    margin: 1px;\n"
    "}\n"
    "textview text {\n"
    "    background-color: #141413;\n"
    "    color: #F8FAFC;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 12px;\n"
    "}\n"
    "progressbar trough {\n"
    "    background-color: #21262D;\n"
    "    border: 1px solid #30363D;\n"
    "    border-radius: 4px;\n"
    "    min-height: 8px;\n"
    "}\n"
    "progressbar progress {\n"
    "    background-image: none;\n"
    "    background-color: #1F6FEB;\n"
    "    border-radius: 4px;\n"
    "}\n"
    ".badge-idle {\n"
    "    background-color: #21262D;\n"
    "    color: #8B949E;\n"
    "    border: 1px solid #30363D;\n"
    "    border-radius: 4px;\n"
    "    padding: 4px 10px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    ".badge-running {\n"
    "    background-color: #1C2D42;\n"
    "    color: #58A6FF;\n"
    "    border: 1px solid #388BFD;\n"
    "    border-radius: 4px;\n"
    "    padding: 4px 10px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    ".badge-verified {\n"
    "    background-color: #102C18;\n"
    "    color: #3FB950;\n"
    "    border: 1px solid #238636;\n"
    "    border-radius: 4px;\n"
    "    padding: 4px 10px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    ".badge-corrupted {\n"
    "    background-color: #3C1E1E;\n"
    "    color: #F85149;\n"
    "    border: 1px solid #DA3633;\n"
    "    border-radius: 4px;\n"
    "    padding: 4px 10px;\n"
    "    font-weight: 700;\n"
    "    font-size: 11.5px;\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "}\n"
    ".mode-synced {\n"
    "    color: #3FB950;\n"
    "    font-weight: 700;\n"
    "    font-size: 12px;\n"
    "}\n"
    ".mode-unsynced {\n"
    "    color: #F85149;\n"
    "    font-weight: 700;\n"
    "    font-size: 12px;\n"
    "}\n"
    "separator {\n"
    "    background-color: #2C2C29;\n"
    "    min-width: 1px;\n"
    "    min-height: 1px;\n"
    "}\n"
    "/* Pages */\n"
    ".history-page, .benchmark-page {\n"
    "    background-color: #141413;\n"
    "    padding: 16px 20px;\n"
    "}\n"
    ".history-header-title, .bm-header-title {\n"
    "    font-size: 17px;\n"
    "    font-weight: 800;\n"
    "    letter-spacing: -0.01em;\n"
    "    color: #F8FAFC;\n"
    "}\n"
    ".history-header-desc, .bm-header-desc {\n"
    "    font-size: 12.5px;\n"
    "    color: #94A3B8;\n"
    "    margin-bottom: 12px;\n"
    "}\n"
    ".history-card-metric, .bm-card {\n"
    "    background-color: #1C1C1A;\n"
    "    border: 1px solid #2C2C29;\n"
    "    border-radius: 8px;\n"
    "    padding: 12px 16px;\n"
    "}\n"
    ".bm-summary-box {\n"
    "    background-color: #1C1C1A;\n"
    "    border: 1px solid #2C2C29;\n"
    "    border-radius: 8px;\n"
    "    padding: 12px 16px;\n"
    "}\n"
    ".history-metric-title {\n"
    "    font-size: 11.5px;\n"
    "    font-weight: 700;\n"
    "    color: #94A3B8;\n"
    "    letter-spacing: 0.03em;\n"
    "}\n"
    ".history-metric-val {\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 22px;\n"
    "    font-weight: 800;\n"
    "    color: #F8FAFC;\n"
    "}\n"
    ".history-metric-val-ok {\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 22px;\n"
    "    font-weight: 800;\n"
    "    color: #3FB950;\n"
    "}\n"
    ".history-metric-val-err {\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\n"
    "    font-size: 22px;\n"
    "    font-weight: 800;\n"
    "    color: #F85149;\n"
    "}\n"
    "treeview.history-tree {\n"
    "    background-color: #1C1C1A;\n"
    "    color: #F8FAFC;\n"
    "    border: 1px solid #2C2C29;\n"
    "    border-radius: 6px;\n"
    "    font-size: 12.5px;\n"
    "}\n"
    "treeview.history-tree:selected {\n"
    "    background-color: #1F6FEB;\n"
    "    color: #FFFFFF;\n"
    "}\n"
    "treeview.history-tree header button {\n"
    "    background-image: none;\n"
    "    background-color: #21262D;\n"
    "    color: #F8FAFC;\n"
    "    font-weight: 700;\n"
    "    font-size: 12px;\n"
    "    border: 1px solid #30363D;\n"
    "    padding: 7px 10px;\n"
    "}\n"
    "treeview.history-tree header button label {\n"
    "    color: #F8FAFC;\n"
    "    font-weight: 700;\n"
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
 * INTERACTIVE MEMORY TOOLTIPS (INSPECTOR POPOVER)
 * ========================================================================= */
gboolean on_query_tooltip_ram_buffer(GtkWidget *widget, gint x, gint y, gboolean keyboard_mode, GtkTooltip *tooltip, gpointer user_data) {
    (void)keyboard_mode;
    GuiDashboard *gui = (GuiDashboard *)user_data;
    AppContext *ctx = gui->ctx;
    int width = gtk_widget_get_allocated_width(widget);

    double margin_x = 20.0;
    double slot_top_y = 32.0;
    double slot_height = 42.0;
    double available_w = width - (2 * margin_x);
    double gap = 3.0;
    double slot_w = (available_w - ((BUFFER_CAPACITY - 1) * gap)) / (double)BUFFER_CAPACITY;

    if (y < slot_top_y || y > slot_top_y + slot_height) return FALSE;
    if (x < margin_x || x > width - margin_x) return FALSE;

    int idx = (int)((x - margin_x) / (slot_w + gap));
    if (idx < 0 || idx >= BUFFER_CAPACITY) return FALSE;

    double slot_x = margin_x + idx * (slot_w + gap);
    if (x > slot_x + slot_w) return FALSE;

    pthread_mutex_lock(&ctx->buffer_mutex);
    bool is_occupied = ctx->buffer.slots[idx].is_occupied;
    int chunk_id = ctx->buffer.slots[idx].chunk_id;
    int producer_id = ctx->buffer.slots[idx].producer_id;
    size_t chunk_sz = ctx->buffer.slots[idx].size;
    off_t byte_off = ctx->buffer.slots[idx].byte_offset;
    int head = ctx->buffer.head;
    int tail = ctx->buffer.tail;
    pthread_mutex_unlock(&ctx->buffer_mutex);

    char markup[512];
    if (is_occupied) {
        snprintf(markup, sizeof(markup),
                 "<b>📦 RAM Bounded Buffer Slot #%02d</b> (Địa chỉ: 0x%02X)\n"
                 "• Trạng thái: <span foreground='#0C66E4' weight='bold'>ĐANG CHỨA CHUNK #%03d</span>\n"
                 "• Luồng nạp (Producer): <b>Producer #%d</b>\n"
                 "• Vị trí byte tệp: <b>0x%08lX (Bytes: %'lu - %'lu)</b>\n"
                 "• Dung lượng khối: <b>%zu KB (%'zu Bytes)</b>\n"
                 "• Con trỏ buffer: %s",
                 idx, idx, chunk_id, producer_id,
                 (unsigned long)byte_off, (unsigned long)byte_off, (unsigned long)(byte_off + chunk_sz - 1),
                 chunk_sz / 1024, chunk_sz,
                 (idx == head && idx == tail) ? "<span foreground='#D97706' weight='bold'>[HEAD &amp; TAIL TRÙNG NHAU]</span>" :
                 (idx == head) ? "<span foreground='#0C66E4' weight='bold'>[HEAD: Vị trí Producer nạp kế tiếp]</span>" :
                 (idx == tail) ? "<span foreground='#16A34A' weight='bold'>[TAIL: Vị trí Consumer rút ghi đĩa]</span>" :
                 "<span>[ĐANG CHỜ TIÊU THỤ]</span>");
    } else {
        snprintf(markup, sizeof(markup),
                 "<b>🔲 RAM Bounded Buffer Slot #%02d</b> (Địa chỉ: 0x%02X)\n"
                 "• Trạng thái: <span foreground='#64748B'>TRỐNG // SẴN SÀNG NHẬN DỮ LIỆU</span>\n"
                 "• Sức chứa tối đa: <b>64 KB (65,536 Bytes)</b>\n"
                 "• Con trỏ buffer: %s",
                 idx, idx,
                 (idx == head && idx == tail) ? "<span foreground='#D97706' weight='bold'>[HEAD &amp; TAIL (Buffer Rỗng)]</span>" :
                 (idx == head) ? "<span foreground='#0C66E4' weight='bold'>[HEAD: Vị trí Producer nạp kế tiếp]</span>" :
                 (idx == tail) ? "<span foreground='#16A34A' weight='bold'>[TAIL: Vị trí Consumer chờ lấy]</span>" :
                 "<span>[SLOT TRỐNG]</span>");
    }

    gtk_tooltip_set_markup(tooltip, markup);
    return TRUE;
}

gboolean on_query_tooltip_block_matrix(GtkWidget *widget, gint x, gint y, gboolean keyboard_mode, GtkTooltip *tooltip, gpointer user_data) {
    (void)keyboard_mode;
    GuiDashboard *gui = (GuiDashboard *)user_data;
    AppContext *ctx = gui->ctx;
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    int cols = 32;
    int rows = 8;
    double margin_x = 16.0;
    double margin_y = 12.0;
    double gap = 2.0;

    double avail_w = width - (2 * margin_x);
    double cell_w = (avail_w - (cols - 1) * gap) / cols;
    double avail_h = height - (2 * margin_y) - 24.0;
    double cell_h = (avail_h - (rows - 1) * gap) / rows;

    if (x < margin_x || y < margin_y) return FALSE;
    if (x > width - margin_x || y > margin_y + rows * (cell_h + gap)) return FALSE;

    int c = (int)((x - margin_x) / (cell_w + gap));
    int r = (int)((y - margin_y) / (cell_h + gap));
    if (c < 0 || c >= cols || r < 0 || r >= rows) return FALSE;

    double cell_x = margin_x + c * (cell_w + gap);
    double cell_y = margin_y + r * (cell_h + gap);
    if (x > cell_x + cell_w || y > cell_y + cell_h) return FALSE;

    int chunk_id = r * cols + c;
    int total = (ctx->total_chunks > 0 && ctx->total_chunks <= MAX_CHUNKS) ? ctx->total_chunks : 256;
    if (chunk_id >= total) return FALSE;

    BlockStatus st = ctx->block_statuses[chunk_id];
    int pid = ctx->block_producer_id[chunk_id];
    int cid = ctx->block_consumer_id[chunk_id];
    uint64_t start_byte = (uint64_t)chunk_id * CHUNK_SIZE;
    uint64_t end_byte = start_byte + CHUNK_SIZE - 1;
    if (ctx->total_file_size > 0 && end_byte >= ctx->total_file_size) {
        end_byte = ctx->total_file_size - 1;
    }
    size_t chunk_bytes = (end_byte >= start_byte) ? (end_byte - start_byte + 1) : CHUNK_SIZE;

    char status_str[256];
    char p_info[128];
    char c_info[128];

    if (pid > 0) snprintf(p_info, sizeof(p_info), "Producer #%d (Tải HTTP Range)", pid);
    else snprintf(p_info, sizeof(p_info), "Chưa chỉ định");

    if (cid > 0) snprintf(c_info, sizeof(c_info), "Consumer #%d (Ghi I/O đĩa)", cid);
    else snprintf(c_info, sizeof(c_info), "Chưa xử lý");

    switch (st) {
        case BLOCK_UNASSIGNED:
            snprintf(status_str, sizeof(status_str), "<span foreground='#64748B'>Chưa gán // Chờ điều phối</span>");
            break;
        case BLOCK_DOWNLOADING:
            snprintf(status_str, sizeof(status_str), "<span foreground='#0C66E4' weight='bold'>⚡ Đang tải qua mạng (Network Ingress)</span>");
            break;
        case BLOCK_QUEUED:
            snprintf(status_str, sizeof(status_str), "<span foreground='#D97706' weight='bold'>⏳ Đã nạp vào RAM Bounded Buffer</span>");
            break;
        case BLOCK_WRITTEN:
            snprintf(status_str, sizeof(status_str), "<span foreground='#16A34A' weight='bold'>✔ ĐÃ GHI ĐĨA AN TOÀN // COMMITTED</span>");
            break;
        case BLOCK_CORRUPTED:
            snprintf(status_str, sizeof(status_str), "<span foreground='#DC2626' weight='bold'>⚠ HỎNG DỮ LIỆU DO RACE CONDITION! (Xung đột lseek)</span>");
            break;
    }

    char markup[640];
    snprintf(markup, sizeof(markup),
             "<b>💽 Ma Trận Khối Đĩa: Block #%03d</b> (Index %d)\n"
             "• Trạng thái: %s\n"
             "• Offset đĩa: <b>0x%08lX</b>\n"
             "• Dải bytes: <b>%'lu - %'lu (%zu KB)</b>\n"
             "• Luồng tải (Producer): <b>%s</b>\n"
             "• Luồng ghi (Consumer): <b>%s</b>%s",
             chunk_id, chunk_id,
             status_str,
             (unsigned long)start_byte,
             (unsigned long)start_byte, (unsigned long)end_byte, chunk_bytes / 1024,
             p_info, c_info,
             (st == BLOCK_CORRUPTED) ? "\n<span foreground='#DC2626' weight='bold'>→ XUNG ĐỘT GHI ĐÈ BỞI CÁC CONSUMERS DO THIẾU MUTEX! DỮ LIỆU BỊ HỎNG.</span>" : "");

    gtk_tooltip_set_markup(tooltip, markup);
    return TRUE;
}

/* =========================================================================
 * CAIRO 2D DRAWING: RAM BOUNDED BUFFER INSPECTOR (TOP CANVAS)
 * ========================================================================= */
/* =========================================================================
 * PANGO CAIRO TYPOGRAPHY HELPERS (CRISP ANTI-ALIASED UNICODE / VIETNAMESE)
 * ========================================================================= */
static void pango_draw_text(cairo_t *cr, double x, double y, const char *font_desc_str, const char *text, double r, double g, double b) {
    if (!text || !*text) return;
    cairo_set_source_rgb(cr, r, g, b);
    PangoLayout *layout = pango_cairo_create_layout(cr);
    PangoFontDescription *desc = pango_font_description_from_string(font_desc_str);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_text(layout, text, -1);
    cairo_move_to(cr, x, y);
    pango_cairo_show_layout(cr, layout);
    pango_font_description_free(desc);
    g_object_unref(layout);
}

static void pango_draw_text_centered(cairo_t *cr, double cx, double cy, const char *font_desc_str, const char *text, double r, double g, double b) {
    if (!text || !*text) return;
    cairo_set_source_rgb(cr, r, g, b);
    PangoLayout *layout = pango_cairo_create_layout(cr);
    PangoFontDescription *desc = pango_font_description_from_string(font_desc_str);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_text(layout, text, -1);
    int p_w, p_h;
    pango_layout_get_pixel_size(layout, &p_w, &p_h);
    cairo_move_to(cr, cx - (p_w / 2.0), cy - (p_h / 2.0));
    pango_cairo_show_layout(cr, layout);
    pango_font_description_free(desc);
    g_object_unref(layout);
}

gboolean on_draw_ram_buffer(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    GuiDashboard *gui = (GuiDashboard *)user_data;
    AppContext *ctx = gui->ctx;

    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    /* Background: Surface Container */
    if (gui->is_dark_mode) {
        cairo_set_source_rgb(cr, 0.086, 0.086, 0.078); /* #161614 */
    } else {
        cairo_set_source_rgb(cr, 0.973, 0.980, 0.988); /* #F8FAFC */
    }
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    /* Border */
    if (gui->is_dark_mode) {
        cairo_set_source_rgb(cr, 0.173, 0.173, 0.161); /* #2C2C29 */
    } else {
        cairo_set_source_rgb(cr, 0.886, 0.910, 0.941); /* #E2E8F0 */
    }
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
        if (gui->is_dark_mode) {
            cairo_set_source_rgb(cr, 0.80, 0.84, 0.89); /* #CBD5E1 */
        } else {
            cairo_set_source_rgb(cr, 0.12, 0.16, 0.23); /* #1E293B */
        }
        cairo_set_font_size(cr, 9.5);
        char hex_addr[8];
        snprintf(hex_addr, sizeof(hex_addr), "%02X", i);
        cairo_text_extents_t aext;
        cairo_text_extents(cr, hex_addr, &aext);
        cairo_move_to(cr, x + (slot_w - aext.width) / 2.0 - aext.x_bearing, slot_top_y - 14.0);
        cairo_show_text(cr, hex_addr);

        /* Slot Body */
        if (is_occupied) {
            /* Jira Blue: Queued in RAM */
            if (gui->is_dark_mode) {
                cairo_set_source_rgb(cr, 0.220, 0.545, 0.992); /* #388BFD */
            } else {
                cairo_set_source_rgb(cr, 0.047, 0.400, 0.894); /* #0C66E4 */
            }
            cairo_rectangle(cr, x, slot_top_y, slot_w, slot_height);
            cairo_fill(cr);

            /* Chunk ID inside slot (Crisp White) */
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
            cairo_set_font_size(cr, 11.5);
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
                cairo_set_source_rgb(cr, 0.937, 0.965, 1.00); /* #EFF6FF */
                cairo_set_font_size(cr, 9.0);
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
            if (gui->is_dark_mode) {
                cairo_set_source_rgb(cr, 0.129, 0.149, 0.176); /* #21262D */
            } else {
                cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); /* Crisp #FFFFFF card */
            }
            cairo_rectangle(cr, x, slot_top_y, slot_w, slot_height);
            cairo_fill(cr);
        }

        /* Slot Hairline Border */
        if (gui->is_dark_mode) {
            cairo_set_source_rgb(cr, 0.188, 0.212, 0.239); /* #30363D */
        } else {
            cairo_set_source_rgb(cr, 0.796, 0.835, 0.882); /* #CBD5E1 */
        }
        cairo_set_line_width(cr, 1.0);
        cairo_rectangle(cr, x + 0.5, slot_top_y + 0.5, slot_w - 1, slot_height - 1);
        cairo_stroke(cr);

        /* Decimal slot index below */
        if (gui->is_dark_mode) {
            cairo_set_source_rgb(cr, 0.80, 0.84, 0.89); /* #CBD5E1 */
        } else {
            cairo_set_source_rgb(cr, 0.12, 0.16, 0.23); /* #1E293B */
        }
        cairo_set_font_size(cr, 9.5);
        char idx_str[8];
        snprintf(idx_str, sizeof(idx_str), "%02d", i);
        cairo_text_extents_t iext;
        cairo_text_extents(cr, idx_str, &iext);
        cairo_move_to(cr, x + (slot_w - iext.width) / 2.0 - iext.x_bearing, slot_top_y + slot_height + 11.0);
        cairo_show_text(cr, idx_str);

        /* =============================================================
         * HEAD (IN) POINTER: JIRA BLUE ARROW ABOVE
         * ============================================================= */
        if (i == head_idx) {
            double center_x = x + slot_w / 2.0;
            if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.220, 0.545, 0.992);
            else cairo_set_source_rgb(cr, 0.047, 0.400, 0.894);

            cairo_move_to(cr, center_x, slot_top_y - 2.0);
            cairo_line_to(cr, center_x - 4.5, slot_top_y - 7.5);
            cairo_line_to(cr, center_x + 4.5, slot_top_y - 7.5);
            cairo_close_path(cr);
            cairo_fill(cr);

            cairo_set_font_size(cr, 10.5);
            const char *head_lbl = "HEAD";
            cairo_text_extents_t hext;
            cairo_text_extents(cr, head_lbl, &hext);
            cairo_move_to(cr, center_x - hext.width / 2.0 - hext.x_bearing, slot_top_y - 8.5);
            cairo_show_text(cr, head_lbl);
        }

        /* =============================================================
         * TAIL (OUT) POINTER: ATLASSIAN GREEN ARROW BELOW
         * ============================================================= */
        if (i == tail_idx) {
            double center_x = x + slot_w / 2.0;
            double arrow_base_y = slot_top_y + slot_height + 14.0;
            if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.247, 0.725, 0.314);
            else cairo_set_source_rgb(cr, 0.086, 0.639, 0.290);

            cairo_move_to(cr, center_x, arrow_base_y);
            cairo_line_to(cr, center_x - 4.5, arrow_base_y + 5.5);
            cairo_line_to(cr, center_x + 4.5, arrow_base_y + 5.5);
            cairo_close_path(cr);
            cairo_fill(cr);

            cairo_set_font_size(cr, 10.5);
            const char *tail_lbl = "TAIL";
            cairo_text_extents_t text;
            cairo_text_extents(cr, tail_lbl, &text);
            cairo_move_to(cr, center_x - text.width / 2.0 - text.x_bearing, arrow_base_y + 15.0);
            cairo_show_text(cr, tail_lbl);
        }
    }

    /* Top Left Status Text */
    if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.941, 0.965, 0.988);
    else cairo_set_source_rgb(cr, 0.059, 0.090, 0.165);
    cairo_set_font_size(cr, 11.5);
    cairo_move_to(cr, margin_x, 14.0);
    cairo_show_text(cr, "CIRCULAR BUFFER MAP // 32 SLOTS (64KB EACH)");

    /* Top Right Occupancy Gauge */
    double pct = (count / (double)BUFFER_CAPACITY) * 100.0;
    char occ_str[64];
    snprintf(occ_str, sizeof(occ_str), "OCCUPANCY: %02d/32 (%.1f%%)", count, pct);
    cairo_text_extents_t oext;
    cairo_text_extents(cr, occ_str, &oext);
    if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.220, 0.545, 0.992);
    else cairo_set_source_rgb(cr, 0.047, 0.400, 0.894);
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

    /* Background Surface */
    if (gui->is_dark_mode) {
        cairo_set_source_rgb(cr, 0.086, 0.086, 0.078); /* #161614 */
    } else {
        cairo_set_source_rgb(cr, 0.973, 0.980, 0.988); /* #F8FAFC */
    }
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    /* Border */
    if (gui->is_dark_mode) {
        cairo_set_source_rgb(cr, 0.173, 0.173, 0.161); /* #2C2C29 */
    } else {
        cairo_set_source_rgb(cr, 0.886, 0.910, 0.941); /* #E2E8F0 */
    }
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
                if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.129, 0.149, 0.176); /* #21262D */
                else cairo_set_source_rgb(cr, 0.902, 0.925, 0.949); /* #E6ECF2 - clearly distinct, soft blueprint cell */
                count_unassigned++;
                break;
            case BLOCK_DOWNLOADING:
                if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.220, 0.545, 0.992);
                else cairo_set_source_rgb(cr, 0.047, 0.400, 0.894); /* #0C66E4 Jira Blue */
                count_downloading++;
                break;
            case BLOCK_QUEUED:
                cairo_set_source_rgb(cr, 0.851, 0.467, 0.024); /* #D97706 Amber */
                count_queued++;
                break;
            case BLOCK_WRITTEN:
                if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.247, 0.725, 0.314);
                else cairo_set_source_rgb(cr, 0.086, 0.639, 0.290); /* #16A34A Green */
                count_written++;
                break;
            case BLOCK_CORRUPTED:
                cairo_set_source_rgb(cr, 0.863, 0.149, 0.149); /* #DC2626 Red */
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
        if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.188, 0.212, 0.239); /* #30363D */
        else cairo_set_source_rgb(cr, 0.886, 0.910, 0.941); /* #E2E8F0 */
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
        {"UNASSIGNED", count_unassigned, gui->is_dark_mode ? 0.129 : 0.945, gui->is_dark_mode ? 0.149 : 0.961, gui->is_dark_mode ? 0.176 : 0.976},
        {"PRODUCER ACTIVE", count_downloading, gui->is_dark_mode ? 0.220 : 0.047, gui->is_dark_mode ? 0.545 : 0.400, gui->is_dark_mode ? 0.992 : 0.894},
        {"QUEUED (RAM)", count_queued, 0.851, 0.467, 0.024},
        {"COMMITTED (SAFE)", count_written, gui->is_dark_mode ? 0.247 : 0.086, gui->is_dark_mode ? 0.725 : 0.639, gui->is_dark_mode ? 0.314 : 0.290},
        {"RACE CORRUPTED", count_corrupted, 0.863, 0.149, 0.149}
    };

    cairo_select_font_face(cr, "Liberation Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 11.5);

    for (int k = 0; k < 5; k++) {
        /* 10x10px square badge */
        cairo_set_source_rgb(cr, legend_entries[k].r, legend_entries[k].g, legend_entries[k].b);
        cairo_rectangle(cr, leg_x, leg_y - 9.0, 10, 10);
        cairo_fill_preserve(cr);
        if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.35, 0.35, 0.35);
        else cairo_set_source_rgb(cr, 0.60, 0.65, 0.75);
        cairo_set_line_width(cr, 1.0);
        cairo_stroke(cr);

        if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.95, 0.97, 0.99); /* #F8FAFC */
        else cairo_set_source_rgb(cr, 0.06, 0.09, 0.16); /* #0F172A */
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
 * CAIRO 2D DRAWING: MULTI-CHANNEL REAL-TIME THROUGHPUT GRAPH (SIDEBAR)
 * Channels: Network Ingress (Producer - Blue) vs Disk Write Egress (Consumer - Green)
 * ========================================================================= */
gboolean on_draw_sidebar_sparkline(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    GuiDashboard *gui = (GuiDashboard *)user_data;

    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    /* Background */
    if (gui->is_dark_mode) {
        cairo_set_source_rgb(cr, 0.078, 0.078, 0.075); /* #141413 */
    } else {
        cairo_set_source_rgb(cr, 0.973, 0.980, 0.988); /* #F8FAFC */
    }
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    /* Border */
    if (gui->is_dark_mode) {
        cairo_set_source_rgb(cr, 0.173, 0.173, 0.161); /* #2C2C29 */
    } else {
        cairo_set_source_rgb(cr, 0.886, 0.910, 0.941); /* #E2E8F0 */
    }
    cairo_set_line_width(cr, 1.0);
    cairo_rectangle(cr, 0.5, 0.5, width - 1, height - 1);
    cairo_stroke(cr);

    /* Reference Grid Lines */
    for (int g = 1; g <= 2; g++) {
        double gy = height - (g * (height / 3.0));
        cairo_move_to(cr, 4, gy);
        cairo_line_to(cr, width - 4, gy);
        cairo_stroke(cr);
    }

    double max_scale = (gui->max_speed_recorded > 10.0) ? (gui->max_speed_recorded * 1.15) : 100.0;
    double step_x = (double)(width - 8) / (double)(SPARKLINE_POINTS - 1);

    /* 1. Network Ingress Curve (Producer - Blue) */
    cairo_set_line_width(cr, 1.8);
    if (gui->is_dark_mode) {
        cairo_set_source_rgb(cr, 0.220, 0.545, 0.992); /* #388BFD */
    } else {
        cairo_set_source_rgb(cr, 0.047, 0.400, 0.894); /* #0C66E4 */
    }

    for (int p = 0; p < SPARKLINE_POINTS; p++) {
        int idx = (gui->speed_history_idx + p) % SPARKLINE_POINTS;
        double val = gui->producer_speed_history[idx];
        double py = height - 4.0 - ((val / max_scale) * (height - 18.0));
        double px = 4.0 + p * step_x;

        if (p == 0) cairo_move_to(cr, px, py);
        else cairo_line_to(cr, px, py);
    }
    cairo_stroke_preserve(cr);

    /* Subtle Gradient Shading Underneath Producer Curve */
    cairo_line_to(cr, width - 4.0, height - 2.0);
    cairo_line_to(cr, 4.0, height - 2.0);
    cairo_close_path(cr);
    if (gui->is_dark_mode) {
        cairo_set_source_rgba(cr, 0.220, 0.545, 0.992, 0.15);
    } else {
        cairo_set_source_rgba(cr, 0.047, 0.400, 0.894, 0.12);
    }
    cairo_fill(cr);

    /* 2. Disk Write Egress Curve (Consumer - Green) */
    cairo_set_line_width(cr, 1.6);
    if (gui->is_dark_mode) {
        cairo_set_source_rgb(cr, 0.247, 0.725, 0.314); /* #3FB950 */
    } else {
        cairo_set_source_rgb(cr, 0.086, 0.639, 0.290); /* #16A34A */
    }

    for (int p = 0; p < SPARKLINE_POINTS; p++) {
        int idx = (gui->speed_history_idx + p) % SPARKLINE_POINTS;
        double val = gui->consumer_speed_history[idx];
        double py = height - 4.0 - ((val / max_scale) * (height - 18.0));
        double px = 4.0 + p * step_x;

        if (p == 0) cairo_move_to(cr, px, py);
        else cairo_line_to(cr, px, py);
    }
    cairo_stroke(cr);

    /* Top Left: Peak Watermark */
    cairo_select_font_face(cr, "Liberation Mono", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 10.5);
    if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.85, 0.90, 0.95);
    else cairo_set_source_rgb(cr, 0.10, 0.15, 0.22);

    char max_lbl[32];
    snprintf(max_lbl, sizeof(max_lbl), "PEAK: %.1f MB/s", gui->max_speed_recorded);
    cairo_move_to(cr, 6, 12.5);
    cairo_show_text(cr, max_lbl);

    /* Top Right: Multi-Channel Legend */
    double leg_x = width - 110;
    if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.220, 0.545, 0.992);
    else cairo_set_source_rgb(cr, 0.047, 0.400, 0.894);
    cairo_rectangle(cr, leg_x, 5.0, 5, 5);
    cairo_fill(cr);
    cairo_set_font_size(cr, 10.0);
    cairo_move_to(cr, leg_x + 8, 12.0);
    cairo_show_text(cr, "Net(P)");

    leg_x += 52;
    if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.247, 0.725, 0.314);
    else cairo_set_source_rgb(cr, 0.086, 0.639, 0.290);
    cairo_rectangle(cr, leg_x, 5.0, 5, 5);
    cairo_fill(cr);
    cairo_set_font_size(cr, 10.0);
    cairo_move_to(cr, leg_x + 8, 12.0);
    cairo_show_text(cr, "Disk(C)");

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

    /* Push speed to history buffer for Sparkline (Multi-Channel Realtime) */
    struct timespec now_ts;
    clock_gettime(CLOCK_MONOTONIC, &now_ts);
    double dt = (now_ts.tv_sec - gui->last_speed_tick.tv_sec) + 
                (now_ts.tv_nsec - gui->last_speed_tick.tv_nsec) / 1e9;
    if (dt >= 0.08) {
        uint64_t cur_dl = ctx->bytes_downloaded;
        uint64_t cur_wr = ctx->bytes_written;
        double p_speed = 0.0;
        double c_speed = 0.0;

        if (ctx->state == STATE_RUNNING) {
            if (cur_dl >= gui->last_bytes_downloaded) {
                p_speed = ((cur_dl - gui->last_bytes_downloaded) / (1024.0 * 1024.0)) / dt;
            }
            if (cur_wr >= gui->last_bytes_written) {
                c_speed = ((cur_wr - gui->last_bytes_written) / (1024.0 * 1024.0)) / dt;
            }
        } else if (ctx->state == STATE_COMPLETED) {
            p_speed = 0.0;
            c_speed = 0.0;
        }

        gui->last_bytes_downloaded = cur_dl;
        gui->last_bytes_written = cur_wr;
        gui->last_speed_tick = now_ts;

        gui->speed_history[gui->speed_history_idx] = speed_mbps;
        gui->producer_speed_history[gui->speed_history_idx] = p_speed;
        gui->consumer_speed_history[gui->speed_history_idx] = c_speed;
        gui->speed_history_idx = (gui->speed_history_idx + 1) % SPARKLINE_POINTS;

        if (p_speed > gui->max_speed_recorded) gui->max_speed_recorded = p_speed;
        if (c_speed > gui->max_speed_recorded) gui->max_speed_recorded = c_speed;
        if (speed_mbps > gui->max_speed_recorded) gui->max_speed_recorded = speed_mbps;
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
        gtk_label_set_text(GTK_LABEL(gui->lbl_source_badge), "[ CHƯA CHỌN TỆP ]");
        gtk_style_context_add_class(badge_sc, "badge-idle");
        if (gui->lbl_target_source) gtk_label_set_text(GTK_LABEL(gui->lbl_target_source), "NOT SELECTED");
    } else if (strstr(text, "benchmark_16mb.bin") || strstr(text, "127.0.0.1")) {
        gtk_label_set_text(GTK_LABEL(gui->lbl_source_badge), "[ TỆP TEST OFFLINE 16MB ]");
        gtk_style_context_add_class(badge_sc, "badge-dataset");
        gtk_style_context_add_class(b_test_sc, "btn-active");
        if (gui->lbl_target_source) gtk_label_set_text(GTK_LABEL(gui->lbl_target_source), "OFFLINE 16 MB");
    } else {
        gtk_label_set_text(GTK_LABEL(gui->lbl_source_badge), "[ LIÊN KẾT INTERNET ]");
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
        gtk_button_set_label(button, "⏸ TẠM DỪNG");
        app_log(ctx, "UI_ACTION", "CONTROL", "Pipeline resumed");
    } else {
        gtk_button_set_label(button, "▶ TIẾP TỤC");
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
    memset(gui->producer_speed_history, 0, sizeof(gui->producer_speed_history));
    memset(gui->consumer_speed_history, 0, sizeof(gui->consumer_speed_history));
    gui->speed_history_idx = 0;
    gui->last_bytes_downloaded = 0;
    gui->last_bytes_written = 0;
    clock_gettime(CLOCK_MONOTONIC, &gui->last_speed_tick);

    gtk_text_buffer_set_text(gui->text_buffer_logs, "", -1);
    gtk_label_set_text(GTK_LABEL(gui->lbl_actual_sha), "----------------------------------------------------------------");
    gtk_button_set_label(GTK_BUTTON(gui->btn_pause), "⏸ TẠM DỪNG");
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
        gtk_label_set_text(GTK_LABEL(gui->lbl_sync_mode), "● ĐÃ BẬT MUTEX & SEMAPHORE (AN TOÀN)");
        gtk_style_context_remove_class(sc, "mode-unsynced");
        gtk_style_context_add_class(sc, "mode-synced");
        app_log(ctx, "UI_ACTION", "CONFIG", "Concurrency mode changed to SYNCHRONIZED (Guarded with Mutex & Semaphores)");
    } else {
        gtk_label_set_text(GTK_LABEL(gui->lbl_sync_mode), "▲ TẮT ĐỒNG BỘ (XẢY RA RACE CONDITION!)");
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
    GtkStyleContext *sc_b = gtk_widget_get_style_context(gui->btn_nav_benchmark);
    gtk_style_context_add_class(sc_w, "nav-item-active");
    gtk_style_context_remove_class(sc_h, "nav-item-active");
    if (sc_b) gtk_style_context_remove_class(sc_b, "nav-item-active");
}

void on_nav_history_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GuiDashboard *gui = (GuiDashboard *)user_data;
    gtk_stack_set_visible_child_name(GTK_STACK(gui->main_stack), "history");

    GtkStyleContext *sc_w = gtk_widget_get_style_context(gui->btn_nav_workspace);
    GtkStyleContext *sc_h = gtk_widget_get_style_context(gui->btn_nav_history);
    GtkStyleContext *sc_b = gtk_widget_get_style_context(gui->btn_nav_benchmark);
    gtk_style_context_remove_class(sc_w, "nav-item-active");
    gtk_style_context_add_class(sc_h, "nav-item-active");
    if (sc_b) gtk_style_context_remove_class(sc_b, "nav-item-active");
}

void on_nav_benchmark_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GuiDashboard *gui = (GuiDashboard *)user_data;
    gtk_stack_set_visible_child_name(GTK_STACK(gui->main_stack), "benchmark");

    GtkStyleContext *sc_w = gtk_widget_get_style_context(gui->btn_nav_workspace);
    GtkStyleContext *sc_h = gtk_widget_get_style_context(gui->btn_nav_history);
    GtkStyleContext *sc_b = gtk_widget_get_style_context(gui->btn_nav_benchmark);
    gtk_style_context_remove_class(sc_w, "nav-item-active");
    gtk_style_context_remove_class(sc_h, "nav-item-active");
    if (sc_b) gtk_style_context_add_class(sc_b, "nav-item-active");
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
 * Modern White & Royal Blue SaaS Styling
 * ========================================================================= */
static GtkWidget* create_navigation_sidebar(GuiDashboard *gui) {
    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(sidebar, 230, -1);
    gtk_style_context_add_class(gtk_widget_get_style_context(sidebar), "nav-sidebar");

    /* App Header / Brand */
    GtkWidget *brand_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(sidebar), brand_box, FALSE, FALSE, 4);

    GtkWidget *brand_badge = gtk_label_new("CL");
    gtk_widget_set_size_request(brand_badge, 34, 34);
    gtk_style_context_add_class(gtk_widget_get_style_context(brand_badge), "brand-badge");
    gtk_box_pack_start(GTK_BOX(brand_box), brand_badge, FALSE, FALSE, 0);

    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_box_pack_start(GTK_BOX(brand_box), title_box, TRUE, TRUE, 0);

    GtkWidget *lbl_brand = gtk_label_new("Concurrency Lab");
    gtk_widget_set_halign(lbl_brand, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_brand), "nav-brand-title");
    gtk_box_pack_start(GTK_BOX(title_box), lbl_brand, FALSE, FALSE, 0);

    GtkWidget *lbl_sub = gtk_label_new("POSIX THREAD ENGINE");
    gtk_widget_set_halign(lbl_sub, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_sub), "nav-brand-subtitle");
    gtk_box_pack_start(GTK_BOX(title_box), lbl_sub, FALSE, FALSE, 0);

    /* Separator */
    GtkWidget *sep_top = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(sidebar), sep_top, FALSE, FALSE, 10);

    /* Section label */
    GtkWidget *lbl_sec = gtk_label_new("DANH MỤC LÀM VIỆC");
    gtk_widget_set_halign(lbl_sec, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_sec), "nav-section-label");
    gtk_box_pack_start(GTK_BOX(sidebar), lbl_sec, FALSE, FALSE, 4);

    /* Menu Item 1: TRANSFER ENGINE (Active by default) */
    gui->btn_nav_workspace = gtk_button_new();
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_nav_workspace), "nav-item");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_nav_workspace), "nav-item-active");

    GtkWidget *box_item1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget *lbl_icon1 = gtk_label_new("⚡");
    gtk_box_pack_start(GTK_BOX(box_item1), lbl_icon1, FALSE, FALSE, 0);
    GtkWidget *lbl_item1 = gtk_label_new("Trình Tải Đa Luồng");
    gtk_widget_set_halign(lbl_item1, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box_item1), lbl_item1, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(gui->btn_nav_workspace), box_item1);
    g_signal_connect(gui->btn_nav_workspace, "clicked", G_CALLBACK(on_nav_workspace_clicked), gui);
    gtk_box_pack_start(GTK_BOX(sidebar), gui->btn_nav_workspace, FALSE, FALSE, 4);

    /* Menu Item 2: DOWNLOAD HISTORY */
    gui->btn_nav_history = gtk_button_new();
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_nav_history), "nav-item");

    GtkWidget *box_item2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget *lbl_icon2 = gtk_label_new("🕒");
    gtk_box_pack_start(GTK_BOX(box_item2), lbl_icon2, FALSE, FALSE, 0);
    GtkWidget *lbl_item2 = gtk_label_new("Lịch Sử Tải Xuống");
    gtk_widget_set_halign(lbl_item2, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box_item2), lbl_item2, TRUE, TRUE, 0);

    gui->lbl_nav_hist_badge = gtk_label_new("0");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_nav_hist_badge), "nav-badge-count");
    gtk_box_pack_end(GTK_BOX(box_item2), gui->lbl_nav_hist_badge, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(gui->btn_nav_history), box_item2);
    g_signal_connect(gui->btn_nav_history, "clicked", G_CALLBACK(on_nav_history_clicked), gui);
    gtk_box_pack_start(GTK_BOX(sidebar), gui->btn_nav_history, FALSE, FALSE, 4);

    /* Menu Item 3: BENCHMARK ARENA */
    gui->btn_nav_benchmark = gtk_button_new();
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_nav_benchmark), "nav-item");

    GtkWidget *box_item3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget *lbl_icon3 = gtk_label_new("⚔");
    gtk_box_pack_start(GTK_BOX(box_item3), lbl_icon3, FALSE, FALSE, 0);
    GtkWidget *lbl_item3 = gtk_label_new("Đấu Trường So Sánh");
    gtk_widget_set_halign(lbl_item3, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box_item3), lbl_item3, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(gui->btn_nav_benchmark), box_item3);
    g_signal_connect(gui->btn_nav_benchmark, "clicked", G_CALLBACK(on_nav_benchmark_clicked), gui);
    gtk_box_pack_start(GTK_BOX(sidebar), gui->btn_nav_benchmark, FALSE, FALSE, 4);

    /* Vertical Spacer to push quick actions to bottom */
    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(sidebar), spacer, TRUE, TRUE, 0);

    /* Theme Toggle Button (Light SaaS / Dark Carbon) */
    gui->btn_theme_toggle = gtk_button_new_with_label(gui->is_dark_mode ? "☀️ Giao diện: Sáng" : "🌙 Giao diện: Tối");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_theme_toggle), "btn-theme-toggle");
    g_signal_connect(gui->btn_theme_toggle, "clicked", G_CALLBACK(on_btn_theme_toggle_clicked), gui);
    gtk_box_pack_start(GTK_BOX(sidebar), gui->btn_theme_toggle, FALSE, FALSE, 6);

    /* Bottom Quick Action: Open test_data/ Storage Folder */
    GtkWidget *btn_open_storage = gtk_button_new_with_label("📁 Mở thư mục tải về");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_open_storage), "nav-btn-action");
    g_signal_connect(btn_open_storage, "clicked", G_CALLBACK(on_btn_open_storage_clicked), gui);
    gtk_box_pack_start(GTK_BOX(sidebar), btn_open_storage, FALSE, FALSE, 6);

    /* Footer info */
    GtkWidget *footer_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_style_context_add_class(gtk_widget_get_style_context(footer_box), "nav-footer-box");
    gtk_box_pack_start(GTK_BOX(sidebar), footer_box, FALSE, FALSE, 0);

    GtkWidget *lbl_f1 = gtk_label_new("LƯU TẠI: test_data/");
    gtk_widget_set_halign(lbl_f1, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_f1), "nav-footer-text");
    gtk_box_pack_start(GTK_BOX(footer_box), lbl_f1, FALSE, FALSE, 0);

    GtkWidget *lbl_f2 = gtk_label_new("HỆ THỐNG: Linux 64-bit");
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

    GtkWidget *lbl_title = gtk_label_new("LỊCH SỬ TẢI XUỐNG & KIỂM ĐỊNH TOÀN VẸN DỮ LIỆU");
    gtk_widget_set_halign(lbl_title, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_title), "history-header-title");
    gtk_box_pack_start(GTK_BOX(header_box), lbl_title, FALSE, FALSE, 0);

    GtkWidget *lbl_desc = gtk_label_new("Nhật ký chi tiết các phiên tải đa luồng, đối soát toàn vẹn SHA-256 và giám sát lỗi Race Condition.");
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
    MAKE_HIST_CARD(metrics_box, "TỔNG SỐ PHIÊN TẢI", gui->lbl_hist_total, "history-metric-val");

    gui->lbl_hist_verified = gtk_label_new("0");
    MAKE_HIST_CARD(metrics_box, "TẢI THÀNH CÔNG (SHA-256 AN TOÀN)", gui->lbl_hist_verified, "history-metric-val-ok");

    gui->lbl_hist_corrupted = gtk_label_new("0");
    MAKE_HIST_CARD(metrics_box, "LỖI DO RACE CONDITION / MẠNG", gui->lbl_hist_corrupted, "history-metric-val-err");

    gui->lbl_hist_volume = gtk_label_new("0.00 MB");
    MAKE_HIST_CARD(metrics_box, "TỔNG DUNG LƯỢNG ĐÃ TẢI", gui->lbl_hist_volume, "history-metric-val");

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
 * THEME ENGINE: TOGGLE BETWEEN JIRA LIGHT AND INDUSTRIAL CARBON DARK
 * ========================================================================= */
void on_btn_theme_toggle_clicked(GtkButton *button, gpointer user_data) {
    GuiDashboard *gui = (GuiDashboard *)user_data;
    gui->is_dark_mode = !gui->is_dark_mode;

    if (gui->is_dark_mode) {
        gtk_button_set_label(button, "☀️ Giao diện: Sáng");
        gtk_css_provider_load_from_data(gui->css_provider, WORKSTATION_CSS_DARK, -1, NULL);
        app_log(gui->ctx, "THEME", "SWITCH", "Chuyển sang Giao diện Tối (Matte Carbon & Cobalt)");
    } else {
        gtk_button_set_label(button, "🌙 Giao diện: Tối");
        gtk_css_provider_load_from_data(gui->css_provider, WORKSTATION_CSS_LIGHT, -1, NULL);
        app_log(gui->ctx, "THEME", "SWITCH", "Chuyển sang Giao diện Sáng (Jira SaaS White-Blue)");
    }

    /* Redraw all dynamic Cairo canvases */
    if (gui->drawing_ram_buffer) gtk_widget_queue_draw(gui->drawing_ram_buffer);
    if (gui->drawing_block_matrix) gtk_widget_queue_draw(gui->drawing_block_matrix);
    if (gui->drawing_sidebar_sparkline) gtk_widget_queue_draw(gui->drawing_sidebar_sparkline);
    if (gui->drawing_bm_chart) gtk_widget_queue_draw(gui->drawing_bm_chart);
}

/* =========================================================================
 * PAGE 3: BENCHMARK ARENA IMPLEMENTATION & WORKER
 * ========================================================================= */
typedef struct {
    GuiDashboard *gui;
    int phase; /* 1 = sync done, 2 = unsync done, -1 = error */
    double sync_time;
    double sync_speed;
    bool sync_verified;
    int sync_errors;
    char sync_sha[65];

    double unsync_time;
    double unsync_speed;
    bool unsync_verified;
    int unsync_errors;
    char unsync_sha[65];

    char status_msg[128];
} BenchmarkProgressData;

static gboolean on_bm_idle_progress_cb(gpointer user_data) {
    BenchmarkProgressData *data = (BenchmarkProgressData *)user_data;
    if (!data || !data->gui) {
        free(data);
        return G_SOURCE_REMOVE;
    }
    GuiDashboard *gui = data->gui;

    gtk_label_set_text(GTK_LABEL(gui->lbl_bm_status), data->status_msg);

    if (data->phase == 1) {
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(gui->progress_bm), 0.50);

        char buf[64];
        snprintf(buf, sizeof(buf), "%.3f s", data->sync_time);
        gtk_label_set_text(GTK_LABEL(gui->lbl_bm_sync_time), buf);

        snprintf(buf, sizeof(buf), "%.2f MB/s", data->sync_speed);
        gtk_label_set_text(GTK_LABEL(gui->lbl_bm_sync_speed), buf);

        snprintf(buf, sizeof(buf), "%d Blocks", data->sync_errors);
        gtk_label_set_text(GTK_LABEL(gui->lbl_bm_sync_errors), buf);

        if (data->sync_verified && data->sync_errors == 0) {
            gtk_label_set_text(GTK_LABEL(gui->lbl_bm_sync_integrity), "✔ KHỚP 100% (AN TOÀN)");
            gtk_style_context_remove_class(gtk_widget_get_style_context(gui->lbl_bm_sync_integrity), "badge-corrupted");
            gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_bm_sync_integrity), "badge-verified");
        } else {
            gtk_label_set_text(GTK_LABEL(gui->lbl_bm_sync_integrity), "✖ SAI LỆCH");
            gtk_style_context_remove_class(gtk_widget_get_style_context(gui->lbl_bm_sync_integrity), "badge-verified");
            gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_bm_sync_integrity), "badge-corrupted");
        }

        gtk_label_set_text(GTK_LABEL(gui->lbl_bm_sync_sha), data->sync_sha);

    } else if (data->phase == 2) {
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(gui->progress_bm), 1.0);

        char buf[64];
        snprintf(buf, sizeof(buf), "%.3f s", data->unsync_time);
        gtk_label_set_text(GTK_LABEL(gui->lbl_bm_unsync_time), buf);

        snprintf(buf, sizeof(buf), "%.2f MB/s", data->unsync_speed);
        gtk_label_set_text(GTK_LABEL(gui->lbl_bm_unsync_speed), buf);

        snprintf(buf, sizeof(buf), "%d Blocks", data->unsync_errors);
        gtk_label_set_text(GTK_LABEL(gui->lbl_bm_unsync_errors), buf);

        if (data->unsync_verified && data->unsync_errors == 0) {
            gtk_label_set_text(GTK_LABEL(gui->lbl_bm_unsync_integrity), "✔ KHỚP 100%");
            gtk_style_context_remove_class(gtk_widget_get_style_context(gui->lbl_bm_unsync_integrity), "badge-corrupted");
            gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_bm_unsync_integrity), "badge-verified");
        } else {
            gtk_label_set_text(GTK_LABEL(gui->lbl_bm_unsync_integrity), "✖ HỎNG DỮ LIỆU (DATA RACE DETECTED)");
            gtk_style_context_remove_class(gtk_widget_get_style_context(gui->lbl_bm_unsync_integrity), "badge-verified");
            gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_bm_unsync_integrity), "badge-corrupted");
        }

        gtk_label_set_text(GTK_LABEL(gui->lbl_bm_unsync_sha), data->unsync_sha);

        gui->bm_has_results = true;
        gui->bm_running = false;
        gtk_widget_set_sensitive(gui->btn_run_benchmark, TRUE);

        if (gui->drawing_bm_chart) {
            gtk_widget_queue_draw(gui->drawing_bm_chart);
        }
    } else if (data->phase == -1) {
        gui->bm_running = false;
        gtk_widget_set_sensitive(gui->btn_run_benchmark, TRUE);
    }

    free(data);
    return G_SOURCE_REMOVE;
}

static void* benchmark_runner_worker(void *arg) {
    GuiDashboard *gui = (GuiDashboard *)arg;
    AppContext bm_ctx;
    app_context_init(&bm_ctx);

    const char *source_file = "test_data/benchmark_16mb.bin";
    size_t file_size = 256 * CHUNK_SIZE; /* 16 MB */
    char exp_sha[65] = {0};

    mkdir("test_data", 0755);
    if (access(source_file, F_OK) != 0) {
        generate_benchmark_file(source_file, file_size, exp_sha);
    } else {
        compute_file_sha256(source_file, exp_sha);
    }
    strncpy(bm_ctx.expected_sha256, exp_sha, sizeof(bm_ctx.expected_sha256));

    int port = 0;
    start_local_http_server(&bm_ctx, source_file, &port);
    probe_remote_target(&bm_ctx, bm_ctx.target_url);

    app_log(gui->ctx, "BENCHMARK", "START", "Khởi chạy Đấu Trường So Sánh Hiệu Năng kép trên tập tin 16 MB...");

    /* --- PASS 1: SYNCHRONIZED (MUTEX + SEMAPHORE) --- */
    bm_ctx.sync_mode = SYNC_MODE_SYNCHRONIZED;
    bm_ctx.num_producers = 4;
    bm_ctx.num_consumers = 2;
    const char *sync_dest = "test_data/bm_sync_out.bin";
    prepare_output_file(&bm_ctx, sync_dest);

    clock_gettime(CLOCK_MONOTONIC, &bm_ctx.start_time);
    bm_ctx.state = STATE_RUNNING;
    start_consumer_pool(&bm_ctx);
    start_producer_pool(&bm_ctx);
    wait_producer_pool(&bm_ctx);
    wait_consumer_pool(&bm_ctx);
    clock_gettime(CLOCK_MONOTONIC, &bm_ctx.end_time);
    bm_ctx.state = STATE_COMPLETED;
    verify_file_integrity(&bm_ctx);

    double sync_elapsed = (bm_ctx.end_time.tv_sec - bm_ctx.start_time.tv_sec) +
                          (bm_ctx.end_time.tv_nsec - bm_ctx.start_time.tv_nsec) / 1e9;
    if (sync_elapsed <= 0.001) sync_elapsed = 0.001;
    double sync_speed = (bm_ctx.bytes_written / (1024.0 * 1024.0)) / sync_elapsed;

    gui->bm_sync_time_val = sync_elapsed;
    gui->bm_sync_speed_val = sync_speed;

    app_log(gui->ctx, "BENCHMARK", "PASS1", "Lượt 1 (Có Khóa): %.3fs, %.2f MB/s, Lỗi: %lu, SHA: %s",
            sync_elapsed, sync_speed, (unsigned long)bm_ctx.race_corrupted_blocks,
            bm_ctx.sha256_verified ? "KHỚP 100%" : "SAI LỆCH");

    BenchmarkProgressData *p1 = (BenchmarkProgressData *)malloc(sizeof(BenchmarkProgressData));
    if (p1) {
        p1->gui = gui;
        p1->phase = 1;
        p1->sync_time = sync_elapsed;
        p1->sync_speed = sync_speed;
        p1->sync_verified = bm_ctx.sha256_verified;
        p1->sync_errors = (int)bm_ctx.race_corrupted_blocks;
        strncpy(p1->sync_sha, bm_ctx.actual_sha256, 64);
        p1->sync_sha[64] = '\0';
        snprintf(p1->status_msg, sizeof(p1->status_msg), "Hoàn thành Lượt 1 (Có Khóa). Đang chạy Lượt 2 (Không Khóa)...");
        g_idle_add(on_bm_idle_progress_cb, p1);
    }

    usleep(150000); /* 150ms pause */

    /* --- PASS 2: UNSYNCHRONIZED (RACE CONDITION) --- */
    app_context_reset(&bm_ctx);
    strncpy(bm_ctx.expected_sha256, exp_sha, sizeof(bm_ctx.expected_sha256));
    snprintf(bm_ctx.target_url, sizeof(bm_ctx.target_url), "http://127.0.0.1:%d/benchmark.bin", port);
    probe_remote_target(&bm_ctx, bm_ctx.target_url);

    bm_ctx.sync_mode = SYNC_MODE_UNSYNCHRONIZED;
    bm_ctx.num_producers = 4;
    bm_ctx.num_consumers = 2;
    const char *unsync_dest = "test_data/bm_unsync_out.bin";
    prepare_output_file(&bm_ctx, unsync_dest);

    clock_gettime(CLOCK_MONOTONIC, &bm_ctx.start_time);
    bm_ctx.state = STATE_RUNNING;
    start_consumer_pool(&bm_ctx);
    start_producer_pool(&bm_ctx);
    wait_producer_pool(&bm_ctx);
    wait_consumer_pool(&bm_ctx);
    clock_gettime(CLOCK_MONOTONIC, &bm_ctx.end_time);
    bm_ctx.state = STATE_COMPLETED;
    verify_file_integrity(&bm_ctx);

    double unsync_elapsed = (bm_ctx.end_time.tv_sec - bm_ctx.start_time.tv_sec) +
                            (bm_ctx.end_time.tv_nsec - bm_ctx.start_time.tv_nsec) / 1e9;
    if (unsync_elapsed <= 0.001) unsync_elapsed = 0.001;
    double unsync_speed = (bm_ctx.bytes_written / (1024.0 * 1024.0)) / unsync_elapsed;

    gui->bm_unsync_time_val = unsync_elapsed;
    gui->bm_unsync_speed_val = unsync_speed;

    app_log(gui->ctx, "BENCHMARK", "PASS2", "Lượt 2 (Không Khóa): %.3fs, %.2f MB/s, Lỗi xung đột: %lu, SHA: %s",
            unsync_elapsed, unsync_speed, (unsigned long)bm_ctx.race_corrupted_blocks,
            bm_ctx.sha256_verified ? "KHỚP" : "HỎNG DO DATA RACE");

    BenchmarkProgressData *p2 = (BenchmarkProgressData *)malloc(sizeof(BenchmarkProgressData));
    if (p2) {
        p2->gui = gui;
        p2->phase = 2;
        p2->unsync_time = unsync_elapsed;
        p2->unsync_speed = unsync_speed;
        p2->unsync_verified = bm_ctx.sha256_verified;
        p2->unsync_errors = (int)bm_ctx.race_corrupted_blocks;
        strncpy(p2->unsync_sha, bm_ctx.actual_sha256, 64);
        p2->unsync_sha[64] = '\0';
        snprintf(p2->status_msg, sizeof(p2->status_msg), "Hoàn tất đo kiểm kép! Xem kết quả đối chiếu bên dưới.");
        g_idle_add(on_bm_idle_progress_cb, p2);
    }

    stop_local_http_server(&bm_ctx);
    app_context_destroy(&bm_ctx);
    return NULL;
}

void on_btn_run_benchmark_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GuiDashboard *gui = (GuiDashboard *)user_data;
    if (gui->bm_running) return;
    if (gui->worker_running) {
        gtk_label_set_text(GTK_LABEL(gui->lbl_bm_status), "⚠ Vui lòng chờ phiên tải ở Trình Tải Đa Luồng hoàn tất trước!");
        return;
    }

    gui->bm_running = true;
    gtk_widget_set_sensitive(gui->btn_run_benchmark, FALSE);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(gui->progress_bm), 0.1);
    gtk_label_set_text(GTK_LABEL(gui->lbl_bm_status), "Đang chuẩn bị dataset kiểm thử 16 MB...");

    pthread_t tid;
    pthread_create(&tid, NULL, benchmark_runner_worker, gui);
    pthread_detach(tid);
}

gboolean on_draw_bm_chart(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    GuiDashboard *gui = (GuiDashboard *)user_data;
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    /* Background and border */
    if (gui->is_dark_mode) {
        cairo_set_source_rgb(cr, 0.098, 0.098, 0.094); /* #181816 */
    } else {
        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); /* #FFFFFF */
    }
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    if (gui->is_dark_mode) {
        cairo_set_source_rgb(cr, 0.173, 0.173, 0.161); /* #2C2C29 */
    } else {
        cairo_set_source_rgb(cr, 0.886, 0.910, 0.941); /* #E2E8F0 */
    }
    cairo_set_line_width(cr, 1.0);
    cairo_rectangle(cr, 0.5, 0.5, width - 1, height - 1);
    cairo_stroke(cr);

    if (!gui->bm_has_results) {
        double tr = gui->is_dark_mode ? 0.80 : 0.25;
        pango_draw_text_centered(cr, width / 2.0, height / 2.0,
            "Liberation Sans, DejaVu Sans, sans-serif Bold 12",
            "CHƯA CÓ DỮ LIỆU ĐO ĐẠC // NHẤN NÚT [BẮT ĐẦU CHẠY SO SÁNH TỰ ĐỘNG] ĐỂ XEM ĐỒ THỊ TRỰC QUAN",
            tr, tr, tr);
        return FALSE;
    }

    double mid_x = width / 2.0;

    /* Separator line between the two metrics */
    if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.173, 0.173, 0.161);
    else cairo_set_source_rgb(cr, 0.886, 0.910, 0.941);
    cairo_move_to(cr, mid_x, 8);
    cairo_line_to(cr, mid_x, height - 8);
    cairo_stroke(cr);

    double title_r = gui->is_dark_mode ? 0.95 : 0.08;
    pango_draw_text(cr, 20, 14, "Liberation Sans, DejaVu Sans, sans-serif Bold 11.5",
        "1. THỜI GIAN HOÀN THÀNH (GIÂY - CÀNG THẤP CÀNG NHANH)", title_r, title_r, title_r);

    pango_draw_text(cr, mid_x + 20, 14, "Liberation Sans, DejaVu Sans, sans-serif Bold 11.5",
        "2. TỐC ĐỘ GHI DỮ LIỆU (MB/s - CÀNG CAO CÀNG TỐT)", title_r, title_r, title_r);

    double max_t = (gui->bm_sync_time_val > gui->bm_unsync_time_val) ? gui->bm_sync_time_val : gui->bm_unsync_time_val;
    if (max_t <= 0.001) max_t = 1.0;
    max_t *= 1.30;

    double max_s = (gui->bm_sync_speed_val > gui->bm_unsync_speed_val) ? gui->bm_sync_speed_val : gui->bm_unsync_speed_val;
    if (max_s <= 0.001) max_s = 100.0;
    max_s *= 1.30;

    double bar_max_w = (mid_x - 180.0);
    if (bar_max_w < 60.0) bar_max_w = 60.0;

    double lbl_color = gui->is_dark_mode ? 0.90 : 0.12;

    /* --- Left: Time Bars --- */
    /* Sync Time Bar */
    double sync_w = (gui->bm_sync_time_val / max_t) * bar_max_w;
    cairo_set_source_rgb(cr, 0.086, 0.639, 0.290); /* #16A34A */
    cairo_rectangle(cr, 115, 42, sync_w, 22);
    cairo_fill(cr);

    pango_draw_text(cr, 20, 45, "Liberation Sans, DejaVu Sans, sans-serif Bold 11.5", "Có khóa:", lbl_color, lbl_color, lbl_color);

    char buf[64];
    snprintf(buf, sizeof(buf), "%.3fs (Khớp SHA-256)", gui->bm_sync_time_val);
    pango_draw_text(cr, 125 + sync_w, 45, "Liberation Mono, DejaVu Sans Mono, monospace Bold 11.5", buf, 0.086, 0.639, 0.290);

    /* Unsync Time Bar */
    double unsync_w = (gui->bm_unsync_time_val / max_t) * bar_max_w;
    cairo_set_source_rgb(cr, 0.863, 0.149, 0.149); /* #DC2626 */
    cairo_rectangle(cr, 115, 76, unsync_w, 22);
    cairo_fill(cr);

    pango_draw_text(cr, 20, 79, "Liberation Sans, DejaVu Sans, sans-serif Bold 11.5", "Không khóa:", lbl_color, lbl_color, lbl_color);

    snprintf(buf, sizeof(buf), "%.3fs (HỎNG HASH)", gui->bm_unsync_time_val);
    pango_draw_text(cr, 125 + unsync_w, 79, "Liberation Mono, DejaVu Sans Mono, monospace Bold 11.5", buf, 0.863, 0.149, 0.149);

    /* --- Right: Speed Bars --- */
    /* Sync Speed Bar */
    double sync_sw = (gui->bm_sync_speed_val / max_s) * bar_max_w;
    cairo_set_source_rgb(cr, 0.047, 0.400, 0.894); /* #0C66E4 */
    cairo_rectangle(cr, mid_x + 125, 42, sync_sw, 22);
    cairo_fill(cr);

    pango_draw_text(cr, mid_x + 20, 45, "Liberation Sans, DejaVu Sans, sans-serif Bold 11.5", "Có khóa:", lbl_color, lbl_color, lbl_color);

    snprintf(buf, sizeof(buf), "%.1f MB/s", gui->bm_sync_speed_val);
    pango_draw_text(cr, mid_x + 135 + sync_sw, 45, "Liberation Mono, DejaVu Sans Mono, monospace Bold 11.5", buf, 0.047, 0.400, 0.894);

    /* Unsync Speed Bar */
    double unsync_sw = (gui->bm_unsync_speed_val / max_s) * bar_max_w;
    cairo_set_source_rgb(cr, 0.851, 0.467, 0.024); /* #D97706 */
    cairo_rectangle(cr, mid_x + 125, 76, unsync_sw, 22);
    cairo_fill(cr);

    pango_draw_text(cr, mid_x + 20, 79, "Liberation Sans, DejaVu Sans, sans-serif Bold 11.5", "Không khóa:", lbl_color, lbl_color, lbl_color);

    snprintf(buf, sizeof(buf), "%.1f MB/s", gui->bm_unsync_speed_val);
    pango_draw_text(cr, mid_x + 135 + unsync_sw, 79, "Liberation Mono, DejaVu Sans Mono, monospace Bold 11.5", buf, 0.851, 0.467, 0.024);

    return FALSE;
}

static GtkWidget* create_benchmark_page(GuiDashboard *gui) {
    GtkWidget *bm_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_style_context_add_class(gtk_widget_get_style_context(bm_box), "benchmark-page");

    /* Header */
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_box_pack_start(GTK_BOX(bm_box), header_box, FALSE, FALSE, 0);

    GtkWidget *lbl_title = gtk_label_new("ĐẤU TRƯỜNG SO SÁNH HIỆU NĂNG // BENCHMARK ARENA");
    gtk_widget_set_halign(lbl_title, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_title), "bm-header-title");
    gtk_box_pack_start(GTK_BOX(header_box), lbl_title, FALSE, FALSE, 0);

    GtkWidget *lbl_desc = gtk_label_new("Kiểm thử kép tự động trên cùng tập tin 16 MB: Đo lường định lượng sự khác biệt giữa Chế độ Có Khóa (Mutex + Semaphore) và Không Khóa (Data Race).");
    gtk_widget_set_halign(lbl_desc, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_desc), "bm-header-desc");
    gtk_box_pack_start(GTK_BOX(header_box), lbl_desc, FALSE, FALSE, 0);

    /* Control & Status Bar */
    GtkWidget *ctrl_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_box_pack_start(GTK_BOX(bm_box), ctrl_bar, FALSE, FALSE, 0);

    gui->btn_run_benchmark = gtk_button_new_with_label("⚡ BẮT ĐẦU CHẠY SO SÁNH TỰ ĐỘNG (DUAL-PASS)");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_run_benchmark), "btn-primary");
    g_signal_connect(gui->btn_run_benchmark, "clicked", G_CALLBACK(on_btn_run_benchmark_clicked), gui);
    gtk_box_pack_start(GTK_BOX(ctrl_bar), gui->btn_run_benchmark, FALSE, FALSE, 0);

    gui->lbl_bm_status = gtk_label_new("Sẵn sàng kiểm thử 2 chế độ...");
    gtk_widget_set_halign(gui->lbl_bm_status, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_bm_status), "telemetry-key");
    gtk_box_pack_start(GTK_BOX(ctrl_bar), gui->lbl_bm_status, TRUE, TRUE, 0);

    gui->progress_bm = gtk_progress_bar_new();
    gtk_widget_set_size_request(gui->progress_bm, 180, -1);
    gtk_box_pack_end(GTK_BOX(ctrl_bar), gui->progress_bm, FALSE, FALSE, 0);

    /* Side-by-side Cards Container */
    GtkWidget *cards_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_box_pack_start(GTK_BOX(bm_box), cards_hbox, FALSE, FALSE, 0);

    /* Card 1: Synchronized Mode */
    GtkWidget *card_sync = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_style_context_add_class(gtk_widget_get_style_context(card_sync), "bm-card-sync");
    gtk_box_pack_start(GTK_BOX(cards_hbox), card_sync, TRUE, TRUE, 0);

    GtkWidget *lbl_sync_head = gtk_label_new("🟢 CHẾ ĐỘ CÓ KHÓA (MUTEX & SEMAPHORE)");
    gtk_widget_set_halign(lbl_sync_head, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_sync_head), "bm-section-title");
    gtk_box_pack_start(GTK_BOX(card_sync), lbl_sync_head, FALSE, FALSE, 0);

    #define BM_ROW(parent, k_str, val_lbl) do { \
        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8); \
        GtkWidget *k = gtk_label_new(k_str); \
        gtk_widget_set_halign(k, GTK_ALIGN_START); \
        gtk_style_context_add_class(gtk_widget_get_style_context(k), "telemetry-key"); \
        gtk_box_pack_start(GTK_BOX(row), k, FALSE, FALSE, 0); \
        gtk_widget_set_halign(val_lbl, GTK_ALIGN_END); \
        gtk_style_context_add_class(gtk_widget_get_style_context(val_lbl), "telemetry-val"); \
        gtk_box_pack_end(GTK_BOX(row), val_lbl, FALSE, FALSE, 0); \
        gtk_box_pack_start(GTK_BOX(parent), row, FALSE, FALSE, 0); \
    } while(0)

    gui->lbl_bm_sync_time = gtk_label_new("-- s");
    BM_ROW(card_sync, "Thời gian thực thi:", gui->lbl_bm_sync_time);

    gui->lbl_bm_sync_speed = gtk_label_new("-- MB/s");
    BM_ROW(card_sync, "Tốc độ trung bình:", gui->lbl_bm_sync_speed);

    gui->lbl_bm_sync_errors = gtk_label_new("0 Blocks");
    BM_ROW(card_sync, "Số khối bị hỏng (Data Race):", gui->lbl_bm_sync_errors);

    gui->lbl_bm_sync_integrity = gtk_label_new("CHƯA CHẠY");
    BM_ROW(card_sync, "Trạng thái toàn vẹn SHA-256:", gui->lbl_bm_sync_integrity);

    GtkWidget *sync_sha_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    GtkWidget *lbl_sync_sha_k = gtk_label_new("Mã băm SHA-256:");
    gtk_widget_set_halign(lbl_sync_sha_k, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_sync_sha_k), "telemetry-key");
    gtk_box_pack_start(GTK_BOX(sync_sha_box), lbl_sync_sha_k, FALSE, FALSE, 0);

    gui->lbl_bm_sync_sha = gtk_label_new("----------------------------------------------------------------");
    gtk_widget_set_halign(gui->lbl_bm_sync_sha, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_bm_sync_sha), "telemetry-val");
    gtk_box_pack_start(GTK_BOX(sync_sha_box), gui->lbl_bm_sync_sha, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(card_sync), sync_sha_box, FALSE, FALSE, 2);

    /* Card 2: Unsynchronized Mode */
    GtkWidget *card_unsync = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_style_context_add_class(gtk_widget_get_style_context(card_unsync), "bm-card-unsync");
    gtk_box_pack_start(GTK_BOX(cards_hbox), card_unsync, TRUE, TRUE, 0);

    GtkWidget *lbl_unsync_head = gtk_label_new("🔴 CHẾ ĐỘ KHÔNG KHÓA (DATA RACE CONDITION)");
    gtk_widget_set_halign(lbl_unsync_head, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_unsync_head), "bm-section-title");
    gtk_box_pack_start(GTK_BOX(card_unsync), lbl_unsync_head, FALSE, FALSE, 0);

    gui->lbl_bm_unsync_time = gtk_label_new("-- s");
    BM_ROW(card_unsync, "Thời gian thực thi:", gui->lbl_bm_unsync_time);

    gui->lbl_bm_unsync_speed = gtk_label_new("-- MB/s");
    BM_ROW(card_unsync, "Tốc độ trung bình:", gui->lbl_bm_unsync_speed);

    gui->lbl_bm_unsync_errors = gtk_label_new("0 Blocks");
    BM_ROW(card_unsync, "Số khối bị hỏng (Data Race):", gui->lbl_bm_unsync_errors);

    gui->lbl_bm_unsync_integrity = gtk_label_new("CHƯA CHẠY");
    BM_ROW(card_unsync, "Trạng thái toàn vẹn SHA-256:", gui->lbl_bm_unsync_integrity);

    GtkWidget *unsync_sha_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    GtkWidget *lbl_unsync_sha_k = gtk_label_new("Mã băm SHA-256:");
    gtk_widget_set_halign(lbl_unsync_sha_k, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_unsync_sha_k), "telemetry-key");
    gtk_box_pack_start(GTK_BOX(unsync_sha_box), lbl_unsync_sha_k, FALSE, FALSE, 0);

    gui->lbl_bm_unsync_sha = gtk_label_new("----------------------------------------------------------------");
    gtk_widget_set_halign(gui->lbl_bm_unsync_sha, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_bm_unsync_sha), "telemetry-val");
    gtk_box_pack_start(GTK_BOX(unsync_sha_box), gui->lbl_bm_unsync_sha, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(card_unsync), unsync_sha_box, FALSE, FALSE, 2);

    /* Comparative Chart Area */
    GtkWidget *chart_lbl = gtk_label_new("BIỂU ĐỒ TRỰC QUAN SO SÁNH HIỆU NĂNG & TOÀN VẸN (CAIRO 2D ENGINE)");
    gtk_widget_set_halign(chart_lbl, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(chart_lbl), "section-header");
    gtk_box_pack_start(GTK_BOX(bm_box), chart_lbl, FALSE, FALSE, 2);

    gui->drawing_bm_chart = gtk_drawing_area_new();
    gtk_widget_set_size_request(gui->drawing_bm_chart, -1, 115);
    g_signal_connect(gui->drawing_bm_chart, "draw", G_CALLBACK(on_draw_bm_chart), gui);
    gtk_box_pack_start(GTK_BOX(bm_box), gui->drawing_bm_chart, FALSE, FALSE, 0);

    /* Summary & Analysis Box */
    GtkWidget *sum_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_style_context_add_class(gtk_widget_get_style_context(sum_box), "bm-summary-box");
    gtk_box_pack_start(GTK_BOX(bm_box), sum_box, TRUE, TRUE, 0);

    GtkWidget *sum_head = gtk_label_new("💡 KẾT LUẬN & PHÂN TÍCH BẢN CHẤT HỆ THỐNG:");
    gtk_widget_set_halign(sum_head, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(sum_head), "bm-section-title");
    gtk_box_pack_start(GTK_BOX(sum_box), sum_head, FALSE, FALSE, 0);

    GtkWidget *sum_t1 = gtk_label_new("1. [An Toàn Tuyệt Đối]: Chế độ Có Khóa bảo vệ con trỏ ghi tệp tin bằng Mutex và đồng bộ nạp/rút bằng Semaphore. Dù có chi phí khóa nhỏ, mã băm SHA-256 luôn khớp 100%.");
    gtk_widget_set_halign(sum_t1, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(sum_box), sum_t1, FALSE, FALSE, 2);

    GtkWidget *sum_t2 = gtk_label_new("2. [Thảm Họa Dữ Liệu]: Chế độ Không Khóa cho phép nhiều Consumer gọi lseek()/write() đồng thời mà không khóa. Tốc độ có thể tương đương hoặc nhanh hơn vài phần trăm nhưng gây xung đột ghi đè, làm hỏng hoàn toàn tệp.");
    gtk_widget_set_halign(sum_t2, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(sum_box), sum_t2, FALSE, FALSE, 2);

    return bm_box;
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
    gui->is_dark_mode = false;
    gui->bm_running = false;
    gui->bm_has_results = false;
    gui->bm_sync_time_val = 0.0;
    gui->bm_unsync_time_val = 0.0;
    gui->bm_sync_speed_val = 0.0;
    gui->bm_unsync_speed_val = 0.0;
    memset(gui->speed_history, 0, sizeof(gui->speed_history));
    memset(gui->producer_speed_history, 0, sizeof(gui->producer_speed_history));
    memset(gui->consumer_speed_history, 0, sizeof(gui->consumer_speed_history));
    gui->speed_history_idx = 0;
    gui->last_bytes_downloaded = 0;
    gui->last_bytes_written = 0;
    clock_gettime(CLOCK_MONOTONIC, &gui->last_speed_tick);

    /* Load Modern Atlassian / Jira SAAS CSS */
    gui->css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(gui->css_provider, WORKSTATION_CSS_LIGHT, -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(gui->css_provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

    /* Main Window: 1140 x 740 */
    gui->main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(gui->main_window),
                         "SYSTEM PROGRAMMING WORKSTATION // MULTI-THREADED DOWNLOAD MANAGER & CONCURRENCY LAB");

    /* Calculate monitor workarea to center cleanly without dock clipping */
    GdkDisplay *display = gdk_display_get_default();
    GdkMonitor *primary = display ? gdk_display_get_primary_monitor(display) : NULL;
    if (!primary && display) primary = gdk_display_get_monitor_at_point(display, 100, 100);
    GdkRectangle workarea = {0, 0, 1920, 1080};
    if (primary) {
        gdk_monitor_get_workarea(primary, &workarea);
    }
    int win_w = 1100;
    int win_h = 660;
    if (workarea.height <= 800) {
        win_w = 1060;
        win_h = workarea.height - 70;
    }
    if (win_w > workarea.width - 30) win_w = workarea.width - 30;
    if (win_h > workarea.height - 30) win_h = workarea.height - 30;
    int win_x = workarea.x + (workarea.width - win_w) / 2;
    int win_y = workarea.y + (workarea.height - win_h) / 2;
    if (win_x < workarea.x + 10) win_x = workarea.x + 10;
    if (win_y < workarea.y + 10) win_y = workarea.y + 10;

    gtk_style_context_add_class(gtk_widget_get_style_context(gui->main_window), "main-window");
    gtk_window_set_default_size(GTK_WINDOW(gui->main_window), win_w, win_h);
    gtk_window_move(GTK_WINDOW(gui->main_window), win_x, win_y);
    g_signal_connect(gui->main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Root Horizontal Box: Sidebar on left, Stack on right */
    GtkWidget *root_h_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_style_context_add_class(gtk_widget_get_style_context(root_h_box), "main-window");
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
     * 1. TOP TOOLBAR (COMPACT 2-ROW HIGH-PERFORMANCE WORKSTATION HEADER)
     * ===================================================================== */
    GtkWidget *toolbar_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_style_context_add_class(gtk_widget_get_style_context(toolbar_box), "top-toolbar");
    gtk_box_pack_start(GTK_BOX(gui->page_workspace), toolbar_box, FALSE, FALSE, 0);

    /* ROW 1: Target URL Entry, Benchmark Generators, and Primary Action Buttons */
    GtkWidget *row1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(toolbar_box), row1, FALSE, FALSE, 0);

    GtkWidget *lbl_url = gtk_label_new("TARGET URL:");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_url), "section-header");
    gtk_box_pack_start(GTK_BOX(row1), lbl_url, FALSE, FALSE, 0);

    gui->entry_url = gtk_entry_new();
    gtk_widget_set_size_request(gui->entry_url, 260, 28);
    gtk_entry_set_placeholder_text(GTK_ENTRY(gui->entry_url), "Dán URL tải về (HTTP/HTTPS) hoặc nhấn [TỰ SINH FILE TEST]");
    gtk_box_pack_start(GTK_BOX(row1), gui->entry_url, TRUE, TRUE, 0);
    g_signal_connect(gui->entry_url, "changed", G_CALLBACK(on_entry_url_changed), gui);

    gui->btn_test_offline = gtk_button_new_with_label("🧪 TỰ SINH FILE TEST");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_test_offline), "btn-steel");
    g_signal_connect(gui->btn_test_offline, "clicked", G_CALLBACK(on_btn_test_offline_clicked), gui);
    gtk_box_pack_start(GTK_BOX(row1), gui->btn_test_offline, FALSE, FALSE, 0);

    gui->btn_auto_tune = gtk_toggle_button_new_with_label("⚡ AUTO-TUNE [BẬT]");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui->btn_auto_tune), TRUE);
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_auto_tune), "btn-auto-on");
    g_signal_connect(gui->btn_auto_tune, "toggled", G_CALLBACK(on_btn_auto_tune_toggled), gui);
    gtk_box_pack_start(GTK_BOX(row1), gui->btn_auto_tune, FALSE, FALSE, 0);

    GtkWidget *sep_act = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_start(GTK_BOX(row1), sep_act, FALSE, FALSE, 4);

    gui->btn_start = gtk_button_new_with_label("▶ BẮT ĐẦU TẢI");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_start), "btn-primary");
    g_signal_connect(gui->btn_start, "clicked", G_CALLBACK(on_btn_start_clicked), gui);
    gtk_box_pack_start(GTK_BOX(row1), gui->btn_start, FALSE, FALSE, 0);

    gui->btn_pause = gtk_button_new_with_label("⏸ TẠM DỪNG");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_pause), "btn-pause");
    g_signal_connect(gui->btn_pause, "clicked", G_CALLBACK(on_btn_pause_clicked), gui);
    gtk_box_pack_start(GTK_BOX(row1), gui->btn_pause, FALSE, FALSE, 0);

    gui->btn_reset = gtk_button_new_with_label("🔄 ĐẶT LẠI");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->btn_reset), "btn-reset");
    g_signal_connect(gui->btn_reset, "clicked", G_CALLBACK(on_btn_reset_clicked), gui);
    gtk_box_pack_start(GTK_BOX(row1), gui->btn_reset, FALSE, FALSE, 0);

    /* ROW 2: Concurrency Controls (Producers/Consumers) & Synchronization Mode Guard */
    GtkWidget *row2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(toolbar_box), row2, FALSE, FALSE, 0);

    GtkWidget *lbl_p = gtk_label_new("PRODUCERS:");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_p), "section-header");
    gtk_box_pack_start(GTK_BOX(row2), lbl_p, FALSE, FALSE, 0);

    gui->spin_producers = gtk_spin_button_new_with_range(1, 8, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(gui->spin_producers), 4);
    gtk_box_pack_start(GTK_BOX(row2), gui->spin_producers, FALSE, FALSE, 0);

    GtkWidget *lbl_c = gtk_label_new("CONSUMERS:");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_c), "section-header");
    gtk_box_pack_start(GTK_BOX(row2), lbl_c, FALSE, FALSE, 0);

    gui->spin_consumers = gtk_spin_button_new_with_range(1, 8, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(gui->spin_consumers), 2);
    gtk_box_pack_start(GTK_BOX(row2), gui->spin_consumers, FALSE, FALSE, 0);

    GtkWidget *sep_sync = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_start(GTK_BOX(row2), sep_sync, FALSE, FALSE, 4);

    GtkWidget *lbl_sync_title = gtk_label_new("CHẾ ĐỘ ĐỒNG BỘ:");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_sync_title), "section-header");
    gtk_box_pack_start(GTK_BOX(row2), lbl_sync_title, FALSE, FALSE, 0);

    gui->switch_sync_mode = gtk_switch_new();
    gtk_switch_set_active(GTK_SWITCH(gui->switch_sync_mode), TRUE);
    g_signal_connect(gui->switch_sync_mode, "state-set", G_CALLBACK(on_switch_sync_toggled), gui);
    gtk_box_pack_start(GTK_BOX(row2), gui->switch_sync_mode, FALSE, FALSE, 0);

    gui->lbl_sync_mode = gtk_label_new("● ĐÃ BẬT MUTEX & SEMAPHORE (AN TOÀN)");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_sync_mode), "mode-synced");
    gtk_box_pack_start(GTK_BOX(row2), gui->lbl_sync_mode, FALSE, FALSE, 0);

    /* Spacer between sync mode and target badge */
    GtkWidget *row2_spacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(row2), row2_spacer, TRUE, TRUE, 0);

    gui->lbl_source_badge = gtk_label_new("[ CHƯA CHỌN TỆP ]");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_source_badge), "badge-idle");
    gtk_box_pack_end(GTK_BOX(row2), gui->lbl_source_badge, FALSE, FALSE, 0);

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
    gtk_widget_set_size_request(gui->drawing_ram_buffer, -1, 105);
    gtk_widget_set_has_tooltip(gui->drawing_ram_buffer, TRUE);
    gtk_widget_add_events(gui->drawing_ram_buffer, GDK_POINTER_MOTION_MASK);
    g_signal_connect(gui->drawing_ram_buffer, "query-tooltip", G_CALLBACK(on_query_tooltip_ram_buffer), gui);
    g_signal_connect(gui->drawing_ram_buffer, "draw", G_CALLBACK(on_draw_ram_buffer), gui);
    gtk_box_pack_start(GTK_BOX(workspace_box), gui->drawing_ram_buffer, FALSE, FALSE, 0);

    /* Bottom Cairo: Disk Block Allocation Matrix */
    GtkWidget *lbl_cairo_bot = gtk_label_new("DISK BLOCK ALLOCATION MATRIX // 256 BLOCKS (16-32 MB STORAGE MAP)");
    gtk_widget_set_halign(lbl_cairo_bot, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_cairo_bot), "section-header");
    gtk_box_pack_start(GTK_BOX(workspace_box), lbl_cairo_bot, FALSE, FALSE, 4);

    gui->drawing_block_matrix = gtk_drawing_area_new();
    gtk_widget_set_size_request(gui->drawing_block_matrix, -1, 155);
    gtk_widget_set_has_tooltip(gui->drawing_block_matrix, TRUE);
    gtk_widget_add_events(gui->drawing_block_matrix, GDK_POINTER_MOTION_MASK);
    g_signal_connect(gui->drawing_block_matrix, "query-tooltip", G_CALLBACK(on_query_tooltip_block_matrix), gui);
    g_signal_connect(gui->drawing_block_matrix, "draw", G_CALLBACK(on_draw_block_matrix), gui);
    gtk_box_pack_start(GTK_BOX(workspace_box), gui->drawing_block_matrix, TRUE, TRUE, 0);

    /* =====================================================================
     * 3. BOTTOM PANEL: SHA-256 AUDIT & COLORIZED KERNEL LOG STREAM
     * ===================================================================== */
    GtkWidget *bot_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_style_context_add_class(gtk_widget_get_style_context(bot_panel), "bottom-panel");
    gtk_box_pack_start(GTK_BOX(gui->page_workspace), bot_panel, FALSE, FALSE, 0);

    /* Checksum Audit Row - Responsive 2-Row Layout */
    GtkWidget *sha_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_box_pack_start(GTK_BOX(bot_panel), sha_vbox, FALSE, FALSE, 0);

    GtkWidget *sha_row1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(sha_vbox), sha_row1, FALSE, FALSE, 0);

    GtkWidget *lbl_exp_tag = gtk_label_new("EXPECTED SHA-256:");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_exp_tag), "section-header");
    gtk_box_pack_start(GTK_BOX(sha_row1), lbl_exp_tag, FALSE, FALSE, 0);

    gui->lbl_expected_sha = gtk_label_new("----------------------------------------------------------------");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_expected_sha), "telemetry-val");
    gtk_box_pack_start(GTK_BOX(sha_row1), gui->lbl_expected_sha, FALSE, FALSE, 0);

    gui->lbl_integrity_badge = gtk_label_new("[ STATUS: STANDBY // IDLE ]");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_integrity_badge), "badge-idle");
    gtk_box_pack_end(GTK_BOX(sha_row1), gui->lbl_integrity_badge, FALSE, FALSE, 0);

    GtkWidget *sha_row2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(sha_vbox), sha_row2, FALSE, FALSE, 0);

    GtkWidget *lbl_act_tag = gtk_label_new("ACTUAL SHA-256:  ");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_act_tag), "section-header");
    gtk_box_pack_start(GTK_BOX(sha_row2), lbl_act_tag, FALSE, FALSE, 0);

    gui->lbl_actual_sha = gtk_label_new("----------------------------------------------------------------");
    gtk_style_context_add_class(gtk_widget_get_style_context(gui->lbl_actual_sha), "telemetry-val");
    gtk_box_pack_start(GTK_BOX(sha_row2), gui->lbl_actual_sha, FALSE, FALSE, 0);

    /* Monospace Technical Log Area with Syntax Highlighting Tags */
    GtkWidget *scrolled_log = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolled_log, -1, 100);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_log), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(bot_panel), scrolled_log, FALSE, FALSE, 0);

    gui->text_view_logs = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(gui->text_view_logs), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(gui->text_view_logs), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(gui->text_view_logs), TRUE);
    gui->text_buffer_logs = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gui->text_view_logs));
    gtk_container_add(GTK_CONTAINER(scrolled_log), gui->text_view_logs);

    /* Setup Text Tags for Syntax Highlighting (Jira / Atlassian Tokens) */
    gui->tag_timestamp = gtk_text_buffer_create_tag(gui->text_buffer_logs, "tag_timestamp",
        "foreground", "#64748B", NULL);
    gui->tag_producer = gtk_text_buffer_create_tag(gui->text_buffer_logs, "tag_producer",
        "foreground", "#0C66E4", "weight", PANGO_WEIGHT_BOLD, NULL);
    gui->tag_consumer = gtk_text_buffer_create_tag(gui->text_buffer_logs, "tag_consumer",
        "foreground", "#16A34A", "weight", PANGO_WEIGHT_BOLD, NULL);
    gui->tag_action_enqueue = gtk_text_buffer_create_tag(gui->text_buffer_logs, "tag_action_enqueue",
        "foreground", "#D97706", NULL);
    gui->tag_action_commit = gtk_text_buffer_create_tag(gui->text_buffer_logs, "tag_action_commit",
        "foreground", "#16A34A", "weight", PANGO_WEIGHT_BOLD, NULL);
    gui->tag_action_race = gtk_text_buffer_create_tag(gui->text_buffer_logs, "tag_action_race",
        "foreground", "#DC2626", "weight", PANGO_WEIGHT_BOLD, NULL);
    gui->tag_action_sys = gtk_text_buffer_create_tag(gui->text_buffer_logs, "tag_action_sys",
        "foreground", "#475569", NULL);

    /* =====================================================================
     * PAGE 2: DOWNLOAD HISTORY & AUDIT VIEW
     * ===================================================================== */
    gui->page_history = create_history_page(gui);
    gtk_stack_add_named(GTK_STACK(gui->main_stack), gui->page_history, "history");

    /* =====================================================================
     * PAGE 3: BENCHMARK & COMPARISON ARENA
     * ===================================================================== */
    gui->page_benchmark = create_benchmark_page(gui);
    gtk_stack_add_named(GTK_STACK(gui->main_stack), gui->page_benchmark, "benchmark");

    /* Default to Transfer Engine Workspace page */
    gtk_stack_set_visible_child_name(GTK_STACK(gui->main_stack), "workspace");

    /* Initial Announcement */
    app_log(ctx, "SYS_INIT", "WORKSTATION", "Native GTK3 Concurrency Workstation initialized. System ready.");

    /* Start High-Frequency Telemetry Timer (30 FPS / ~33ms) */
    gui->timer_id = g_timeout_add(33, on_gui_telemetry_tick, gui);

    gtk_widget_show_all(gui->main_window);
    return 0;
}
