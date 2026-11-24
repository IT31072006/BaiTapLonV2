#!/usr/bin/env python3
import re

with open('src/gui_dashboard.c', 'r') as f:
    code = f.read()

# 1. Add #include <pango/pangocairo.h> if missing
if '<pango/pangocairo.h>' not in code:
    code = code.replace('#include <math.h>\n', '#include <math.h>\n#include <pango/pangocairo.h>\n', 1)

# 2. Update WORKSTATION_CSS_LIGHT
css_light_new = """static const char *WORKSTATION_CSS_LIGHT = 
    "* {\\n"
    "    font-family: 'Liberation Sans', 'DejaVu Sans', 'Noto Sans', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Ubuntu, Arial, sans-serif;\\n"
    "    font-size: 13px;\\n"
    "    color: #0F172A;\\n"
    "}\\n"
    "window, .main-window {\\n"
    "    background-color: #F8FAFC;\\n"
    "}\\n"
    "button, button:hover, button:active, button:checked, button:focus, button:backdrop {\\n"
    "    background-image: none;\\n"
    "    box-shadow: none;\\n"
    "    text-shadow: none;\\n"
    "    outline: none;\\n"
    "    border-style: solid;\\n"
    "}\\n"
    "button label {\\n"
    "    color: inherit;\\n"
    "}\\n"
    "/* Left Navigation Sidebar (White & Royal Blue SaaS Style) */\\n"
    ".nav-sidebar {\\n"
    "    background-color: #F0F4FA;\\n"
    "    border-right: 1px solid #D0DFEF;\\n"
    "    padding: 16px 12px;\\n"
    "}\\n"
    ".brand-badge {\\n"
    "    background-color: #0C66E4;\\n"
    "    color: #FFFFFF;\\n"
    "    font-size: 14px;\\n"
    "    font-weight: 800;\\n"
    "    border-radius: 7px;\\n"
    "    padding: 7px 11px;\\n"
    "}\\n"
    ".nav-brand-title {\\n"
    "    font-size: 15.5px;\\n"
    "    font-weight: 800;\\n"
    "    color: #0F172A;\\n"
    "}\\n"
    ".nav-brand-subtitle {\\n"
    "    font-size: 11px;\\n"
    "    font-weight: 700;\\n"
    "    color: #0C66E4;\\n"
    "}\\n"
    ".nav-section-label {\\n"
    "    font-size: 11.5px;\\n"
    "    font-weight: 700;\\n"
    "    color: #334155;\\n"
    "    margin-top: 14px;\\n"
    "    margin-bottom: 6px;\\n"
    "    padding-left: 6px;\\n"
    "}\\n"
    "button.nav-item {\\n"
    "    background-image: none;\\n"
    "    background-color: transparent;\\n"
    "    border: 1px solid transparent;\\n"
    "    border-radius: 7px;\\n"
    "    padding: 8px 12px;\\n"
    "}\\n"
    "button.nav-item label {\\n"
    "    color: #1E293B;\\n"
    "    font-size: 13px;\\n"
    "    font-weight: 600;\\n"
    "}\\n"
    "button.nav-item:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #E2ECF8;\\n"
    "    border: 1px solid #CBD5E1;\\n"
    "}\\n"
    "button.nav-item:hover label {\\n"
    "    color: #0C66E4;\\n"
    "}\\n"
    "button.nav-item-active {\\n"
    "    background-image: none;\\n"
    "    background-color: #0C66E4;\\n"
    "    border: 1px solid #0055CC;\\n"
    "    border-radius: 7px;\\n"
    "}\\n"
    "button.nav-item-active label {\\n"
    "    color: #FFFFFF;\\n"
    "    font-weight: 700;\\n"
    "}\\n"
    ".nav-badge-count {\\n"
    "    background-color: #E2ECF8;\\n"
    "    color: #0C66E4;\\n"
    "    border: 1px solid #B3D4FF;\\n"
    "    border-radius: 9999px;\\n"
    "    padding: 2px 8px;\\n"
    "    font-size: 11px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-weight: 700;\\n"
    "}\\n"
    "button.nav-item-active .nav-badge-count {\\n"
    "    background-color: rgba(255, 255, 255, 0.25);\\n"
    "    color: #FFFFFF;\\n"
    "    border: 1px solid rgba(255, 255, 255, 0.35);\\n"
    "}\\n"
    "button.nav-btn-action {\\n"
    "    background-image: none;\\n"
    "    background-color: #FFFFFF;\\n"
    "    border: 1px solid #0C66E4;\\n"
    "    border-radius: 7px;\\n"
    "    padding: 7px 12px;\\n"
    "}\\n"
    "button.nav-btn-action label {\\n"
    "    color: #0C66E4;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 12px;\\n"
    "}\\n"
    "button.nav-btn-action:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #0C66E4;\\n"
    "    border-color: #0055CC;\\n"
    "}\\n"
    "button.nav-btn-action:hover label {\\n"
    "    color: #FFFFFF;\\n"
    "}\\n"
    "button.btn-theme-toggle {\\n"
    "    background-image: none;\\n"
    "    background-color: #FFFFFF;\\n"
    "    border: 1px solid #CBD5E1;\\n"
    "    border-radius: 7px;\\n"
    "    padding: 6px 12px;\\n"
    "}\\n"
    "button.btn-theme-toggle label {\\n"
    "    color: #334155;\\n"
    "    font-weight: 600;\\n"
    "    font-size: 12px;\\n"
    "}\\n"
    "button.btn-theme-toggle:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #F1F5F9;\\n"
    "    border-color: #94A3B8;\\n"
    "}\\n"
    "button.btn-theme-toggle:hover label {\\n"
    "    color: #0F172A;\\n"
    "}\\n"
    ".nav-footer-box {\\n"
    "    background-color: #FFFFFF;\\n"
    "    border: 1px solid #D0DFEF;\\n"
    "    border-radius: 7px;\\n"
    "    padding: 8px 10px;\\n"
    "    margin-top: 8px;\\n"
    "}\\n"
    ".nav-footer-text {\\n"
    "    font-size: 11px;\\n"
    "    color: #334155;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-weight: 600;\\n"
    "}\\n"
    "/* Panels */\\n"
    ".top-toolbar {\\n"
    "    background-color: #FFFFFF;\\n"
    "    border-bottom: 1px solid #E2E8F0;\\n"
    "    padding: 8px 14px;\\n"
    "}\\n"
    ".sidebar-panel {\\n"
    "    background-color: #FFFFFF;\\n"
    "    border-right: 1px solid #E2E8F0;\\n"
    "    padding: 10px 12px;\\n"
    "}\\n"
    ".workspace-panel {\\n"
    "    background-color: #F8FAFC;\\n"
    "    padding: 10px 14px;\\n"
    "}\\n"
    ".bottom-panel {\\n"
    "    background-color: #FFFFFF;\\n"
    "    border-top: 1px solid #E2E8F0;\\n"
    "    padding: 8px 14px;\\n"
    "}\\n"
    "/* Telemetry Cards */\\n"
    ".telemetry-card {\\n"
    "    background-color: #FFFFFF;\\n"
    "    border: 1px solid #E2E8F0;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 6px 10px;\\n"
    "    margin-bottom: 5px;\\n"
    "}\\n"
    ".section-header {\\n"
    "    font-size: 12.5px;\\n"
    "    font-weight: 700;\\n"
    "    color: #0F172A;\\n"
    "}\\n"
    ".telemetry-key {\\n"
    "    font-size: 12px;\\n"
    "    font-weight: 600;\\n"
    "    color: #334155;\\n"
    "}\\n"
    ".telemetry-val {\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 14px;\\n"
    "    font-weight: 700;\\n"
    "    color: #0F172A;\\n"
    "    font-feature-settings: 'tnum';\\n"
    "}\\n"
    ".telemetry-val-large {\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 20px;\\n"
    "    font-weight: 800;\\n"
    "    color: #0C66E4;\\n"
    "    font-feature-settings: 'tnum';\\n"
    "}\\n"
    ".telemetry-val-alert {\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 14.5px;\\n"
    "    font-weight: 700;\\n"
    "    color: #DC2626;\\n"
    "    font-feature-settings: 'tnum';\\n"
    "}\\n"
    ".telemetry-val-success {\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 14px;\\n"
    "    font-weight: 700;\\n"
    "    color: #16A34A;\\n"
    "}\\n"
    "/* Action Buttons */\\n"
    "button.btn-primary {\\n"
    "    background-image: none;\\n"
    "    background-color: #0C66E4;\\n"
    "    border: 1px solid #0055CC;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 6px 14px;\\n"
    "}\\n"
    "button.btn-primary label {\\n"
    "    color: #FFFFFF;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 12.5px;\\n"
    "}\\n"
    "button.btn-primary:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #0055CC;\\n"
    "    border-color: #0047AB;\\n"
    "}\\n"
    "button.btn-secondary {\\n"
    "    background-image: none;\\n"
    "    background-color: #FFFFFF;\\n"
    "    border: 1px solid #CBD5E1;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 6px 12px;\\n"
    "}\\n"
    "button.btn-secondary label {\\n"
    "    color: #1E293B;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 12.5px;\\n"
    "}\\n"
    "button.btn-secondary:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #F8FAFC;\\n"
    "    border-color: #94A3B8;\\n"
    "}\\n"
    "button.btn-secondary:hover label {\\n"
    "    color: #0F172A;\\n"
    "}\\n"
    "button.btn-pause {\\n"
    "    background-image: none;\\n"
    "    background-color: #FFFFFF;\\n"
    "    border: 1px solid #CBD5E1;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 6px 12px;\\n"
    "}\\n"
    "button.btn-pause label {\\n"
    "    color: #334155;\\n"
    "    font-weight: 600;\\n"
    "    font-size: 12.5px;\\n"
    "}\\n"
    "button.btn-pause:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #FEF3C7;\\n"
    "    border-color: #FDE68A;\\n"
    "}\\n"
    "button.btn-pause:hover label {\\n"
    "    color: #B45309;\\n"
    "}\\n"
    "button.btn-reset {\\n"
    "    background-image: none;\\n"
    "    background-color: #FFFFFF;\\n"
    "    border: 1px solid #CBD5E1;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 6px 12px;\\n"
    "}\\n"
    "button.btn-reset label {\\n"
    "    color: #334155;\\n"
    "    font-weight: 600;\\n"
    "    font-size: 12.5px;\\n"
    "}\\n"
    "button.btn-reset:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #FEE2E2;\\n"
    "    border-color: #FECACA;\\n"
    "}\\n"
    "button.btn-reset:hover label {\\n"
    "    color: #DC2626;\\n"
    "}\\n"
    "button.btn-steel {\\n"
    "    background-image: none;\\n"
    "    background-color: #EFF6FF;\\n"
    "    border: 1px solid #B3D4FF;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 6px 12px;\\n"
    "}\\n"
    "button.btn-steel label {\\n"
    "    color: #0C66E4;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    "button.btn-steel:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #DBEAFE;\\n"
    "    border-color: #93C5FD;\\n"
    "}\\n"
    "button.btn-steel:hover label {\\n"
    "    color: #0055CC;\\n"
    "}\\n"
    "button.btn-active {\\n"
    "    background-image: none;\\n"
    "    background-color: #0C66E4;\\n"
    "    border: 1px solid #0055CC;\\n"
    "}\\n"
    "button.btn-active label {\\n"
    "    color: #FFFFFF;\\n"
    "    font-weight: 700;\\n"
    "}\\n"
    "button.btn-auto-on {\\n"
    "    background-image: none;\\n"
    "    background-color: #EFF6FF;\\n"
    "    border: 1px solid #93C5FD;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 5px 10px;\\n"
    "}\\n"
    "button.btn-auto-on label {\\n"
    "    color: #0C66E4;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    "button.btn-auto-off {\\n"
    "    background-image: none;\\n"
    "    background-color: #F8FAFC;\\n"
    "    border: 1px solid #CBD5E1;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 5px 10px;\\n"
    "}\\n"
    "button.btn-auto-off label {\\n"
    "    color: #475569;\\n"
    "    font-weight: 600;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    ".badge-dataset {\\n"
    "    background-color: #EFF6FF;\\n"
    "    color: #0C66E4;\\n"
    "    border: 1px solid #B3D4FF;\\n"
    "    border-radius: 4px;\\n"
    "    padding: 4px 9px;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    ".badge-dataset-net {\\n"
    "    background-color: #FFFBEB;\\n"
    "    color: #D97706;\\n"
    "    border: 1px solid #FDE68A;\\n"
    "    border-radius: 4px;\\n"
    "    padding: 4px 9px;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    "entry {\\n"
    "    background-image: none;\\n"
    "    background-color: #FFFFFF;\\n"
    "    color: #0F172A;\\n"
    "    border: 1px solid #CBD5E1;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 6px 10px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 12.5px;\\n"
    "}\\n"
    "entry:focus {\\n"
    "    border-color: #0C66E4;\\n"
    "}\\n"
    "spinbutton {\\n"
    "    background-image: none;\\n"
    "    background-color: #FFFFFF;\\n"
    "    border: 1px solid #CBD5E1;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 3px 5px;\\n"
    "}\\n"
    "spinbutton entry {\\n"
    "    border: none;\\n"
    "    background-color: transparent;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 13px;\\n"
    "    color: #0C66E4;\\n"
    "}\\n"
    "spinbutton button {\\n"
    "    background-image: none;\\n"
    "    background-color: #F1F5F9;\\n"
    "    border: none;\\n"
    "    color: #334155;\\n"
    "}\\n"
    "spinbutton button:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #E2E8F0;\\n"
    "    color: #0F172A;\\n"
    "}\\n"
    "switch {\\n"
    "    background-image: none;\\n"
    "    background-color: #CBD5E1;\\n"
    "    border-radius: 12px;\\n"
    "}\\n"
    "switch:checked {\\n"
    "    background-image: none;\\n"
    "    background-color: #16A34A;\\n"
    "}\\n"
    "switch slider {\\n"
    "    background-image: none;\\n"
    "    background-color: #FFFFFF;\\n"
    "    border-radius: 10px;\\n"
    "    min-width: 18px;\\n"
    "    min-height: 18px;\\n"
    "    margin: 1px;\\n"
    "}\\n"
    "textview text {\\n"
    "    background-color: #F8FAFC;\\n"
    "    color: #0F172A;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 12px;\\n"
    "}\\n"
    "progressbar trough {\\n"
    "    background-color: #E2E8F0;\\n"
    "    border: 1px solid #CBD5E1;\\n"
    "    border-radius: 4px;\\n"
    "    min-height: 8px;\\n"
    "}\\n"
    "progressbar progress {\\n"
    "    background-image: none;\\n"
    "    background-color: #0C66E4;\\n"
    "    border-radius: 4px;\\n"
    "}\\n"
    ".badge-idle {\\n"
    "    background-color: #F1F5F9;\\n"
    "    color: #475569;\\n"
    "    border: 1px solid #CBD5E1;\\n"
    "    border-radius: 4px;\\n"
    "    padding: 4px 10px;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    ".badge-running {\\n"
    "    background-color: #EFF6FF;\\n"
    "    color: #0C66E4;\\n"
    "    border: 1px solid #B3D4FF;\\n"
    "    border-radius: 4px;\\n"
    "    padding: 4px 10px;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    ".badge-verified {\\n"
    "    background-color: #F0FDF4;\\n"
    "    color: #16A34A;\\n"
    "    border: 1px solid #BBF7D0;\\n"
    "    border-radius: 4px;\\n"
    "    padding: 4px 10px;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    ".badge-corrupted {\\n"
    "    background-color: #FEF2F2;\\n"
    "    color: #DC2626;\\n"
    "    border: 1px solid #FECACA;\\n"
    "    border-radius: 4px;\\n"
    "    padding: 4px 10px;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    ".mode-synced {\\n"
    "    color: #16A34A;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 12px;\\n"
    "}\\n"
    ".mode-unsynced {\\n"
    "    color: #DC2626;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 12px;\\n"
    "}\\n"
    "separator {\\n"
    "    background-color: #E2E8F0;\\n"
    "    min-width: 1px;\\n"
    "    min-height: 1px;\\n"
    "}\\n"
    "/* Pages */\\n"
    ".history-page, .benchmark-page {\\n"
    "    background-color: #F8FAFC;\\n"
    "    padding: 16px 20px;\\n"
    "}\\n"
    ".history-header-title, .bm-header-title {\\n"
    "    font-size: 17px;\\n"
    "    font-weight: 800;\\n"
    "    letter-spacing: -0.01em;\\n"
    "    color: #0F172A;\\n"
    "}\\n"
    ".history-header-desc, .bm-header-desc {\\n"
    "    font-size: 12.5px;\\n"
    "    color: #334155;\\n"
    "    margin-bottom: 12px;\\n"
    "}\\n"
    ".history-card-metric, .bm-card {\\n"
    "    background-color: #FFFFFF;\\n"
    "    border: 1px solid #E2E8F0;\\n"
    "    border-radius: 8px;\\n"
    "    padding: 12px 16px;\\n"
    "}\\n"
    ".bm-summary-box {\\n"
    "    background-color: #FFFFFF;\\n"
    "    border: 1px solid #E2E8F0;\\n"
    "    border-radius: 8px;\\n"
    "    padding: 12px 16px;\\n"
    "}\\n"
    ".history-metric-title {\\n"
    "    font-size: 11.5px;\\n"
    "    font-weight: 700;\\n"
    "    color: #475569;\\n"
    "    letter-spacing: 0.03em;\\n"
    "}\\n"
    ".history-metric-val {\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 22px;\\n"
    "    font-weight: 800;\\n"
    "    color: #0F172A;\\n"
    "}\\n"
    ".history-metric-val-ok {\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 22px;\\n"
    "    font-weight: 800;\\n"
    "    color: #16A34A;\\n"
    "}\\n"
    ".history-metric-val-err {\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 22px;\\n"
    "    font-weight: 800;\\n"
    "    color: #DC2626;\\n"
    "}\\n"
    "treeview.history-tree {\\n"
    "    background-color: #FFFFFF;\\n"
    "    color: #0F172A;\\n"
    "    border: 1px solid #E2E8F0;\\n"
    "    border-radius: 6px;\\n"
    "    font-size: 12.5px;\\n"
    "}\\n"
    "treeview.history-tree:selected {\\n"
    "    background-color: #EFF6FF;\\n"
    "    color: #0C66E4;\\n"
    "}\\n"
    "treeview.history-tree header button {\\n"
    "    background-image: none;\\n"
    "    background-color: #F1F5F9;\\n"
    "    color: #0F172A;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 12px;\\n"
    "    border: 1px solid #E2E8F0;\\n"
    "    padding: 7px 10px;\\n"
    "}\\n"
    "treeview.history-tree header button label {\\n"
    "    color: #0F172A;\\n"
    "    font-weight: 700;\\n"
    "}\\n";"""

# 3. Update WORKSTATION_CSS_DARK
css_dark_new = """static const char *WORKSTATION_CSS_DARK = 
    "* {\\n"
    "    font-family: 'Liberation Sans', 'DejaVu Sans', 'Noto Sans', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Ubuntu, Arial, sans-serif;\\n"
    "    font-size: 13px;\\n"
    "    color: #F8FAFC;\\n"
    "}\\n"
    "window, .main-window {\\n"
    "    background-color: #141413;\\n"
    "}\\n"
    "button, button:hover, button:active, button:checked, button:focus, button:backdrop {\\n"
    "    background-image: none;\\n"
    "    box-shadow: none;\\n"
    "    text-shadow: none;\\n"
    "    outline: none;\\n"
    "    border-style: solid;\\n"
    "}\\n"
    "button label {\\n"
    "    color: inherit;\\n"
    "}\\n"
    ".nav-sidebar {\\n"
    "    background-color: #1C1C1A;\\n"
    "    border-right: 1px solid #2C2C29;\\n"
    "    padding: 16px 12px;\\n"
    "}\\n"
    ".brand-badge {\\n"
    "    background-color: #1F6FEB;\\n"
    "    color: #FFFFFF;\\n"
    "    font-size: 14px;\\n"
    "    font-weight: 800;\\n"
    "    border-radius: 7px;\\n"
    "    padding: 7px 11px;\\n"
    "}\\n"
    ".nav-brand-title {\\n"
    "    font-size: 15.5px;\\n"
    "    font-weight: 800;\\n"
    "    color: #F8FAFC;\\n"
    "}\\n"
    ".nav-brand-subtitle {\\n"
    "    font-size: 11px;\\n"
    "    font-weight: 700;\\n"
    "    color: #58A6FF;\\n"
    "}\\n"
    ".nav-section-label {\\n"
    "    font-size: 11.5px;\\n"
    "    font-weight: 700;\\n"
    "    color: #94A3B8;\\n"
    "    margin-top: 14px;\\n"
    "    margin-bottom: 6px;\\n"
    "    padding-left: 6px;\\n"
    "}\\n"
    "button.nav-item {\\n"
    "    background-image: none;\\n"
    "    background-color: transparent;\\n"
    "    border: 1px solid transparent;\\n"
    "    border-radius: 7px;\\n"
    "    padding: 8px 12px;\\n"
    "}\\n"
    "button.nav-item label {\\n"
    "    color: #CBD5E1;\\n"
    "    font-size: 13px;\\n"
    "    font-weight: 600;\\n"
    "}\\n"
    "button.nav-item:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #262624;\\n"
    "    border: 1px solid #30363D;\\n"
    "}\\n"
    "button.nav-item:hover label {\\n"
    "    color: #58A6FF;\\n"
    "}\\n"
    "button.nav-item-active {\\n"
    "    background-image: none;\\n"
    "    background-color: #1F6FEB;\\n"
    "    border: 1px solid #388BFD;\\n"
    "    border-radius: 7px;\\n"
    "}\\n"
    "button.nav-item-active label {\\n"
    "    color: #FFFFFF;\\n"
    "    font-weight: 700;\\n"
    "}\\n"
    ".nav-badge-count {\\n"
    "    background-color: #21262D;\\n"
    "    color: #58A6FF;\\n"
    "    border: 1px solid #30363D;\\n"
    "    border-radius: 9999px;\\n"
    "    padding: 2px 8px;\\n"
    "    font-size: 11px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-weight: 700;\\n"
    "}\\n"
    "button.nav-item-active .nav-badge-count {\\n"
    "    background-color: rgba(255, 255, 255, 0.25);\\n"
    "    color: #FFFFFF;\\n"
    "    border: 1px solid rgba(255, 255, 255, 0.35);\\n"
    "}\\n"
    "button.nav-btn-action {\\n"
    "    background-image: none;\\n"
    "    background-color: #21262D;\\n"
    "    border: 1px solid #388BFD;\\n"
    "    border-radius: 7px;\\n"
    "    padding: 7px 12px;\\n"
    "}\\n"
    "button.nav-btn-action label {\\n"
    "    color: #58A6FF;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 12px;\\n"
    "}\\n"
    "button.nav-btn-action:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #1F6FEB;\\n"
    "    border-color: #388BFD;\\n"
    "}\\n"
    "button.nav-btn-action:hover label {\\n"
    "    color: #FFFFFF;\\n"
    "}\\n"
    "button.btn-theme-toggle {\\n"
    "    background-image: none;\\n"
    "    background-color: #21262D;\\n"
    "    border: 1px solid #30363D;\\n"
    "    border-radius: 7px;\\n"
    "    padding: 6px 12px;\\n"
    "}\\n"
    "button.btn-theme-toggle label {\\n"
    "    color: #CBD5E1;\\n"
    "    font-weight: 600;\\n"
    "    font-size: 12px;\\n"
    "}\\n"
    "button.btn-theme-toggle:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #30363D;\\n"
    "    border-color: #58A6FF;\\n"
    "}\\n"
    "button.btn-theme-toggle:hover label {\\n"
    "    color: #F8FAFC;\\n"
    "}\\n"
    ".nav-footer-box {\\n"
    "    background-color: #1C1C1A;\\n"
    "    border: 1px solid #2C2C29;\\n"
    "    border-radius: 7px;\\n"
    "    padding: 8px 10px;\\n"
    "    margin-top: 8px;\\n"
    "}\\n"
    ".nav-footer-text {\\n"
    "    font-size: 11px;\\n"
    "    color: #94A3B8;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-weight: 600;\\n"
    "}\\n"
    "/* Panels */\\n"
    ".top-toolbar {\\n"
    "    background-color: #1C1C1A;\\n"
    "    border-bottom: 1px solid #2C2C29;\\n"
    "    padding: 8px 14px;\\n"
    "}\\n"
    ".sidebar-panel {\\n"
    "    background-color: #1C1C1A;\\n"
    "    border-right: 1px solid #2C2C29;\\n"
    "    padding: 10px 12px;\\n"
    "}\\n"
    ".workspace-panel {\\n"
    "    background-color: #141413;\\n"
    "    padding: 10px 14px;\\n"
    "}\\n"
    ".bottom-panel {\\n"
    "    background-color: #1C1C1A;\\n"
    "    border-top: 1px solid #2C2C29;\\n"
    "    padding: 8px 14px;\\n"
    "}\\n"
    "/* Telemetry Cards */\\n"
    ".telemetry-card {\\n"
    "    background-color: #1C1C1A;\\n"
    "    border: 1px solid #2C2C29;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 6px 10px;\\n"
    "    margin-bottom: 5px;\\n"
    "}\\n"
    ".section-header {\\n"
    "    font-size: 12.5px;\\n"
    "    font-weight: 700;\\n"
    "    color: #F8FAFC;\\n"
    "}\\n"
    ".telemetry-key {\\n"
    "    font-size: 12px;\\n"
    "    font-weight: 600;\\n"
    "    color: #94A3B8;\\n"
    "}\\n"
    ".telemetry-val {\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 14px;\\n"
    "    font-weight: 700;\\n"
    "    color: #F8FAFC;\\n"
    "    font-feature-settings: 'tnum';\\n"
    "}\\n"
    ".telemetry-val-large {\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 20px;\\n"
    "    font-weight: 800;\\n"
    "    color: #58A6FF;\\n"
    "    font-feature-settings: 'tnum';\\n"
    "}\\n"
    ".telemetry-val-alert {\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 14.5px;\\n"
    "    font-weight: 700;\\n"
    "    color: #F85149;\\n"
    "    font-feature-settings: 'tnum';\\n"
    "}\\n"
    ".telemetry-val-success {\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 14px;\\n"
    "    font-weight: 700;\\n"
    "    color: #3FB950;\\n"
    "}\\n"
    "/* Action Buttons */\\n"
    "button.btn-primary {\\n"
    "    background-image: none;\\n"
    "    background-color: #1F6FEB;\\n"
    "    border: 1px solid #388BFD;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 6px 14px;\\n"
    "}\\n"
    "button.btn-primary label {\\n"
    "    color: #FFFFFF;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 12.5px;\\n"
    "}\\n"
    "button.btn-primary:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #388BFD;\\n"
    "    border-color: #58A6FF;\\n"
    "}\\n"
    "button.btn-secondary {\\n"
    "    background-image: none;\\n"
    "    background-color: #21262D;\\n"
    "    border: 1px solid #30363D;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 6px 12px;\\n"
    "}\\n"
    "button.btn-secondary label {\\n"
    "    color: #C9D1D9;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 12.5px;\\n"
    "}\\n"
    "button.btn-secondary:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #30363D;\\n"
    "    border-color: #8B949E;\\n"
    "}\\n"
    "button.btn-secondary:hover label {\\n"
    "    color: #F8FAFC;\\n"
    "}\\n"
    "button.btn-pause {\\n"
    "    background-image: none;\\n"
    "    background-color: #21262D;\\n"
    "    border: 1px solid #30363D;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 6px 12px;\\n"
    "}\\n"
    "button.btn-pause label {\\n"
    "    color: #C9D1D9;\\n"
    "    font-weight: 600;\\n"
    "    font-size: 12.5px;\\n"
    "}\\n"
    "button.btn-pause:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #342508;\\n"
    "    border-color: #D29922;\\n"
    "}\\n"
    "button.btn-pause:hover label {\\n"
    "    color: #F0883E;\\n"
    "}\\n"
    "button.btn-reset {\\n"
    "    background-image: none;\\n"
    "    background-color: #21262D;\\n"
    "    border: 1px solid #30363D;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 6px 12px;\\n"
    "}\\n"
    "button.btn-reset label {\\n"
    "    color: #C9D1D9;\\n"
    "    font-weight: 600;\\n"
    "    font-size: 12.5px;\\n"
    "}\\n"
    "button.btn-reset:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #3C1E1E;\\n"
    "    border-color: #F85149;\\n"
    "}\\n"
    "button.btn-reset:hover label {\\n"
    "    color: #FF7B72;\\n"
    "}\\n"
    "button.btn-steel {\\n"
    "    background-image: none;\\n"
    "    background-color: #161B22;\\n"
    "    border: 1px solid #30363D;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 6px 12px;\\n"
    "}\\n"
    "button.btn-steel label {\\n"
    "    color: #58A6FF;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    "button.btn-steel:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #21262D;\\n"
    "    border-color: #58A6FF;\\n"
    "}\\n"
    "button.btn-steel:hover label {\\n"
    "    color: #79C0FF;\\n"
    "}\\n"
    "button.btn-active {\\n"
    "    background-image: none;\\n"
    "    background-color: #1F6FEB;\\n"
    "    border: 1px solid #388BFD;\\n"
    "}\\n"
    "button.btn-active label {\\n"
    "    color: #FFFFFF;\\n"
    "    font-weight: 700;\\n"
    "}\\n"
    "button.btn-auto-on {\\n"
    "    background-image: none;\\n"
    "    background-color: #1C2D42;\\n"
    "    border: 1px solid #388BFD;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 5px 10px;\\n"
    "}\\n"
    "button.btn-auto-on label {\\n"
    "    color: #58A6FF;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    "button.btn-auto-off {\\n"
    "    background-image: none;\\n"
    "    background-color: #21262D;\\n"
    "    border: 1px solid #30363D;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 5px 10px;\\n"
    "}\\n"
    "button.btn-auto-off label {\\n"
    "    color: #8B949E;\\n"
    "    font-weight: 600;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    ".badge-dataset {\\n"
    "    background-color: #161B22;\\n"
    "    color: #58A6FF;\\n"
    "    border: 1px solid #30363D;\\n"
    "    border-radius: 4px;\\n"
    "    padding: 4px 9px;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    ".badge-dataset-net {\\n"
    "    background-color: #2D2200;\\n"
    "    color: #D29922;\\n"
    "    border: 1px solid #6E4E00;\\n"
    "    border-radius: 4px;\\n"
    "    padding: 4px 9px;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    "entry {\\n"
    "    background-image: none;\\n"
    "    background-color: #161B22;\\n"
    "    color: #F8FAFC;\\n"
    "    border: 1px solid #30363D;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 6px 10px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 12.5px;\\n"
    "}\\n"
    "entry:focus {\\n"
    "    border-color: #58A6FF;\\n"
    "}\\n"
    "spinbutton {\\n"
    "    background-image: none;\\n"
    "    background-color: #161B22;\\n"
    "    border: 1px solid #30363D;\\n"
    "    border-radius: 6px;\\n"
    "    padding: 3px 5px;\\n"
    "}\\n"
    "spinbutton entry {\\n"
    "    border: none;\\n"
    "    background-color: transparent;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 13px;\\n"
    "    color: #58A6FF;\\n"
    "}\\n"
    "spinbutton button {\\n"
    "    background-image: none;\\n"
    "    background-color: #21262D;\\n"
    "    border: none;\\n"
    "    color: #C9D1D9;\\n"
    "}\\n"
    "spinbutton button:hover {\\n"
    "    background-image: none;\\n"
    "    background-color: #30363D;\\n"
    "    color: #F8FAFC;\\n"
    "}\\n"
    "switch {\\n"
    "    background-image: none;\\n"
    "    background-color: #30363D;\\n"
    "    border-radius: 12px;\\n"
    "}\\n"
    "switch:checked {\\n"
    "    background-image: none;\\n"
    "    background-color: #238636;\\n"
    "}\\n"
    "switch slider {\\n"
    "    background-image: none;\\n"
    "    background-color: #F0F6FC;\\n"
    "    border-radius: 10px;\\n"
    "    min-width: 18px;\\n"
    "    min-height: 18px;\\n"
    "    margin: 1px;\\n"
    "}\\n"
    "textview text {\\n"
    "    background-color: #141413;\\n"
    "    color: #F8FAFC;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 12px;\\n"
    "}\\n"
    "progressbar trough {\\n"
    "    background-color: #21262D;\\n"
    "    border: 1px solid #30363D;\\n"
    "    border-radius: 4px;\\n"
    "    min-height: 8px;\\n"
    "}\\n"
    "progressbar progress {\\n"
    "    background-image: none;\\n"
    "    background-color: #1F6FEB;\\n"
    "    border-radius: 4px;\\n"
    "}\\n"
    ".badge-idle {\\n"
    "    background-color: #21262D;\\n"
    "    color: #8B949E;\\n"
    "    border: 1px solid #30363D;\\n"
    "    border-radius: 4px;\\n"
    "    padding: 4px 10px;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    ".badge-running {\\n"
    "    background-color: #1C2D42;\\n"
    "    color: #58A6FF;\\n"
    "    border: 1px solid #388BFD;\\n"
    "    border-radius: 4px;\\n"
    "    padding: 4px 10px;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    ".badge-verified {\\n"
    "    background-color: #102C18;\\n"
    "    color: #3FB950;\\n"
    "    border: 1px solid #238636;\\n"
    "    border-radius: 4px;\\n"
    "    padding: 4px 10px;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    ".badge-corrupted {\\n"
    "    background-color: #3C1E1E;\\n"
    "    color: #F85149;\\n"
    "    border: 1px solid #DA3633;\\n"
    "    border-radius: 4px;\\n"
    "    padding: 4px 10px;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 11.5px;\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "}\\n"
    ".mode-synced {\\n"
    "    color: #3FB950;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 12px;\\n"
    "}\\n"
    ".mode-unsynced {\\n"
    "    color: #F85149;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 12px;\\n"
    "}\\n"
    "separator {\\n"
    "    background-color: #2C2C29;\\n"
    "    min-width: 1px;\\n"
    "    min-height: 1px;\\n"
    "}\\n"
    "/* Pages */\\n"
    ".history-page, .benchmark-page {\\n"
    "    background-color: #141413;\\n"
    "    padding: 16px 20px;\\n"
    "}\\n"
    ".history-header-title, .bm-header-title {\\n"
    "    font-size: 17px;\\n"
    "    font-weight: 800;\\n"
    "    letter-spacing: -0.01em;\\n"
    "    color: #F8FAFC;\\n"
    "}\\n"
    ".history-header-desc, .bm-header-desc {\\n"
    "    font-size: 12.5px;\\n"
    "    color: #94A3B8;\\n"
    "    margin-bottom: 12px;\\n"
    "}\\n"
    ".history-card-metric, .bm-card {\\n"
    "    background-color: #1C1C1A;\\n"
    "    border: 1px solid #2C2C29;\\n"
    "    border-radius: 8px;\\n"
    "    padding: 12px 16px;\\n"
    "}\\n"
    ".bm-summary-box {\\n"
    "    background-color: #1C1C1A;\\n"
    "    border: 1px solid #2C2C29;\\n"
    "    border-radius: 8px;\\n"
    "    padding: 12px 16px;\\n"
    "}\\n"
    ".history-metric-title {\\n"
    "    font-size: 11.5px;\\n"
    "    font-weight: 700;\\n"
    "    color: #94A3B8;\\n"
    "    letter-spacing: 0.03em;\\n"
    "}\\n"
    ".history-metric-val {\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 22px;\\n"
    "    font-weight: 800;\\n"
    "    color: #F8FAFC;\\n"
    "}\\n"
    ".history-metric-val-ok {\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 22px;\\n"
    "    font-weight: 800;\\n"
    "    color: #3FB950;\\n"
    "}\\n"
    ".history-metric-val-err {\\n"
    "    font-family: 'Liberation Mono', 'DejaVu Sans Mono', 'Noto Mono', monospace;\\n"
    "    font-size: 22px;\\n"
    "    font-weight: 800;\\n"
    "    color: #F85149;\\n"
    "}\\n"
    "treeview.history-tree {\\n"
    "    background-color: #1C1C1A;\\n"
    "    color: #F8FAFC;\\n"
    "    border: 1px solid #2C2C29;\\n"
    "    border-radius: 6px;\\n"
    "    font-size: 12.5px;\\n"
    "}\\n"
    "treeview.history-tree:selected {\\n"
    "    background-color: #1F6FEB;\\n"
    "    color: #FFFFFF;\\n"
    "}\\n"
    "treeview.history-tree header button {\\n"
    "    background-image: none;\\n"
    "    background-color: #21262D;\\n"
    "    color: #F8FAFC;\\n"
    "    font-weight: 700;\\n"
    "    font-size: 12px;\\n"
    "    border: 1px solid #30363D;\\n"
    "    padding: 7px 10px;\\n"
    "}\\n"
    "treeview.history-tree header button label {\\n"
    "    color: #F8FAFC;\\n"
    "    font-weight: 700;\\n"
    "}\\n";"""

# Replace CSS blocks
# Find from 'static const char *WORKSTATION_CSS_LIGHT =' to 'static const char *WORKSTATION_CSS_DARK ='
m1 = code.find('static const char *WORKSTATION_CSS_LIGHT =')
m2 = code.find('static const char *WORKSTATION_CSS_DARK =')
m3 = code.find('/* =========================================================================\n * ASYNC IDLE HANDLER FOR HISTORY UPDATES')

if m1 != -1 and m2 != -1 and m3 != -1:
    code = code[:m1] + css_light_new + "\n\n" + css_dark_new + "\n\n" + code[m3:]
    print("Successfully replaced WORKSTATION_CSS_LIGHT and WORKSTATION_CSS_DARK")
else:
    print("Error locating CSS blocks:", m1, m2, m3)

# 4. Add Pango text helpers before on_draw_ram_buffer
pango_helpers = """/* =========================================================================
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

"""

if 'pango_draw_text' not in code:
    code = code.replace('gboolean on_draw_ram_buffer(', pango_helpers + 'gboolean on_draw_ram_buffer(', 1)
    print("Added Pango helper functions")

# 5. Enhance on_draw_ram_buffer fonts and contrast
old_rb = """        /* Micro memory address label above (0x00, 0x01 ... 0x1F) */
        if (gui->is_dark_mode) {
            cairo_set_source_rgb(cr, 0.545, 0.580, 0.620); /* #8B949E */
        } else {
            cairo_set_source_rgb(cr, 0.392, 0.455, 0.545); /* #64748B */
        }
        cairo_set_font_size(cr, 7.5);"""

new_rb = """        /* Micro memory address label above (0x00, 0x01 ... 0x1F) */
        if (gui->is_dark_mode) {
            cairo_set_source_rgb(cr, 0.80, 0.84, 0.89); /* #CBD5E1 */
        } else {
            cairo_set_source_rgb(cr, 0.12, 0.16, 0.23); /* #1E293B */
        }
        cairo_set_font_size(cr, 9.5);"""

code = code.replace(old_rb, new_rb, 1)

code = code.replace("cairo_set_font_size(cr, 9.0);\n            char cid_str[16];",
                    "cairo_set_font_size(cr, 11.5);\n            char cid_str[16];", 1)

code = code.replace("cairo_set_font_size(cr, 7.5);\n                char pid_str[16];",
                    "cairo_set_font_size(cr, 9.0);\n                char pid_str[16];", 1)

old_idx = """        /* Decimal slot index below */
        if (gui->is_dark_mode) {
            cairo_set_source_rgb(cr, 0.545, 0.580, 0.620); /* #8B949E */
        } else {
            cairo_set_source_rgb(cr, 0.392, 0.455, 0.545); /* #64748B */
        }
        cairo_set_font_size(cr, 8.0);"""

new_idx = """        /* Decimal slot index below */
        if (gui->is_dark_mode) {
            cairo_set_source_rgb(cr, 0.80, 0.84, 0.89); /* #CBD5E1 */
        } else {
            cairo_set_source_rgb(cr, 0.12, 0.16, 0.23); /* #1E293B */
        }
        cairo_set_font_size(cr, 9.5);"""

code = code.replace(old_idx, new_idx, 1)

code = code.replace("cairo_set_font_size(cr, 8.5);\n            const char *head_lbl = \"HEAD\";",
                    "cairo_set_font_size(cr, 10.5);\n            const char *head_lbl = \"HEAD\";", 1)

code = code.replace("cairo_set_font_size(cr, 8.5);\n            const char *tail_lbl = \"TAIL\";",
                    "cairo_set_font_size(cr, 10.5);\n            const char *tail_lbl = \"TAIL\";", 1)

code = code.replace("cairo_set_font_size(cr, 9.5);\n    cairo_move_to(cr, margin_x, 14.0);\n    cairo_show_text(cr, \"CIRCULAR BUFFER MAP // 32 SLOTS (64KB EACH)\");",
                    "cairo_set_font_size(cr, 11.5);\n    cairo_move_to(cr, margin_x, 14.0);\n    cairo_show_text(cr, \"CIRCULAR BUFFER MAP // 32 SLOTS (64KB EACH)\");", 1)

# 6. Update on_draw_block_matrix legend font size
old_mat_leg = """    cairo_select_font_face(cr, \"sans-serif\", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 9.0);

    for (int k = 0; k < 5; k++) {
        /* 8x8px square badge */
        cairo_set_source_rgb(cr, legend_entries[k].r, legend_entries[k].g, legend_entries[k].b);
        cairo_rectangle(cr, leg_x, leg_y - 7.5, 8, 8);
        cairo_fill_preserve(cr);
        if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.25, 0.25, 0.25);
        else cairo_set_source_rgb(cr, 0.796, 0.835, 0.882);
        cairo_set_line_width(cr, 0.8);
        cairo_stroke(cr);

        if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.545, 0.580, 0.620); /* #8B949E */
        else cairo_set_source_rgb(cr, 0.278, 0.333, 0.412); /* #475569 */"""

new_mat_leg = """    cairo_select_font_face(cr, \"Liberation Sans\", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
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
        else cairo_set_source_rgb(cr, 0.06, 0.09, 0.16); /* #0F172A */"""

code = code.replace(old_mat_leg, new_mat_leg, 1)

# 7. Update on_draw_sidebar_sparkline font size
old_sp = """    cairo_select_font_face(cr, \"monospace\", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 8.0);
    if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.545, 0.580, 0.620);
    else cairo_set_source_rgb(cr, 0.392, 0.455, 0.545);

    char max_lbl[32];
    snprintf(max_lbl, sizeof(max_lbl), \"PEAK: %.1f MB/s\", gui->max_speed_recorded);
    cairo_move_to(cr, 6, 11);
    cairo_show_text(cr, max_lbl);"""

new_sp = """    cairo_select_font_face(cr, \"Liberation Mono\", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 10.5);
    if (gui->is_dark_mode) cairo_set_source_rgb(cr, 0.85, 0.90, 0.95);
    else cairo_set_source_rgb(cr, 0.10, 0.15, 0.22);

    char max_lbl[32];
    snprintf(max_lbl, sizeof(max_lbl), \"PEAK: %.1f MB/s\", gui->max_speed_recorded);
    cairo_move_to(cr, 6, 12.5);
    cairo_show_text(cr, max_lbl);"""

code = code.replace(old_sp, new_sp, 1)

code = code.replace("cairo_move_to(cr, leg_x + 8, 10.5);\n    cairo_show_text(cr, \"Net(P)\");",
                    "cairo_set_font_size(cr, 10.0);\n    cairo_move_to(cr, leg_x + 8, 12.0);\n    cairo_show_text(cr, \"Net(P)\");", 1)

code = code.replace("cairo_move_to(cr, leg_x + 8, 10.5);\n    cairo_show_text(cr, \"Disk(C)\");",
                    "cairo_set_font_size(cr, 10.0);\n    cairo_move_to(cr, leg_x + 8, 12.0);\n    cairo_show_text(cr, \"Disk(C)\");", 1)

# 8. Refactor on_draw_bm_chart to use PangoCairo for perfect Vietnamese text rendering
bm_chart_start = code.find('gboolean on_draw_bm_chart(GtkWidget *widget, cairo_t *cr, gpointer user_data) {')
bm_chart_end = code.find('static GtkWidget* create_benchmark_page(GuiDashboard *gui) {')

if bm_chart_start != -1 and bm_chart_end != -1:
    bm_chart_new = """gboolean on_draw_bm_chart(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
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

"""
    code = code[:bm_chart_start] + bm_chart_new + code[bm_chart_end:]
    print("Successfully replaced on_draw_bm_chart with PangoCairo rendering")
else:
    print("Error locating on_draw_bm_chart:", bm_chart_start, bm_chart_end)

with open('src/gui_dashboard.c', 'w') as f:
    f.write(code)

print("Finished patching src/gui_dashboard.c!")
