# =============================================================================
# PROJECT: High-Performance Multi-threaded Download Manager & Concurrency Lab
# ARCHITECTURE: Linux Systems Programming - Producer-Consumer Bounded Buffer
# =============================================================================

CC ?= gcc
CFLAGS ?= -Wall -Wextra -pthread -O2 -g -std=gnu11
CPPFLAGS += -I./include

# Directory hierarchy
SRCDIR = src
INCDIR = include
BUILDDIR = build
BINDIR = bin
TESTDIR = test_data

# Detection of system libraries via pkg-config with robust fallback paths
PKG_CONFIG ?= pkg-config

# Check if pkg-config is available
HAS_PKG_CONFIG := $(shell which $(PKG_CONFIG) 2>/dev/null)

ifneq ($(HAS_PKG_CONFIG),)
    GTK_CFLAGS := $(shell $(PKG_CONFIG) --cflags gtk+-3.0 2>/dev/null)
    GTK_LIBS   := $(shell $(PKG_CONFIG) --libs gtk+-3.0 2>/dev/null)
    CURL_CFLAGS:= $(shell $(PKG_CONFIG) --cflags libcurl 2>/dev/null)
    CURL_LIBS  := $(shell $(PKG_CONFIG) --libs libcurl 2>/dev/null)
    SSL_CFLAGS := $(shell $(PKG_CONFIG) --cflags openssl 2>/dev/null)
    SSL_LIBS   := $(shell $(PKG_CONFIG) --libs openssl 2>/dev/null)
endif

# Fallbacks if pkg-config returns empty (e.g., custom environment paths)
ifeq ($(CURL_LIBS),)
    ifneq ($(wildcard /opt/lampp/include/curl/curl.h),)
        CURL_CFLAGS := -I/opt/lampp/include
        CURL_LIBS   := -L/opt/lampp/lib -lcurl -Wl,-rpath,/opt/lampp/lib
    else
        CURL_LIBS   := -lcurl
    endif
endif

ifeq ($(SSL_LIBS),)
    SSL_LIBS := -lssl -lcrypto
endif

ifeq ($(GTK_LIBS),)
    GTK_CFLAGS := -I/usr/include/gtk-3.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pango-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/atk-1.0
    GTK_LIBS   := -lgtk-3 -lgdk-3 -lcairo -lgobject-2.0 -lglib-2.0
endif

# Combined Flags
APP_CFLAGS = $(CFLAGS) $(CPPFLAGS) $(GTK_CFLAGS) $(CURL_CFLAGS) $(SSL_CFLAGS)
APP_LIBS   = -pthread $(GTK_LIBS) $(CURL_LIBS) $(SSL_LIBS) -lm

CORE_CFLAGS = $(CFLAGS) $(CPPFLAGS) $(CURL_CFLAGS) $(SSL_CFLAGS)
CORE_LIBS   = -pthread $(CURL_LIBS) $(SSL_LIBS) -lm

# Source files
CORE_SRCS = $(SRCDIR)/common.c \
            $(SRCDIR)/network_producer.c \
            $(SRCDIR)/storage_consumer.c

CORE_OBJS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(CORE_SRCS))

GUI_SRCS  = $(SRCDIR)/gui_dashboard.c \
            $(SRCDIR)/main.c

GUI_OBJS  = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(GUI_SRCS))

CLI_TEST_SRC = $(SRCDIR)/cli_benchmark.c
CLI_TEST_OBJ = $(BUILDDIR)/cli_benchmark.o

# Binaries
GUI_TARGET = $(BINDIR)/sysp_downloader
CLI_TARGET = $(BINDIR)/cli_benchmark

.PHONY: all clean benchmark test help

all: directories $(CLI_TARGET)
	@echo "======================================================================"
	@echo "Part 1 Core Engine Build Successful: $(CLI_TARGET)"
	@echo "Run './bin/cli_benchmark' to verify SYNCHRONIZED mode."
	@echo "Run './bin/cli_benchmark --race' to verify UNSYNCHRONIZED race mode."
	@echo "Part 2 will compile the Native GTK3 Workstation GUI: $(GUI_TARGET)"
	@echo "======================================================================"

directories:
	@mkdir -p $(BUILDDIR) $(BINDIR) $(TESTDIR)

# Core Object Files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@echo "  CC      $<"
	@$(CC) $(CORE_CFLAGS) -c $< -o $@

# GUI Target (Compiled in Part 2)
gui: directories $(CORE_OBJS) $(GUI_OBJS)
	@echo "  LD      $(GUI_TARGET)"
	@$(CC) $(CORE_OBJS) $(GUI_OBJS) $(APP_LIBS) -o $(GUI_TARGET)
	@echo "GUI Build Complete: $(GUI_TARGET)"

$(BUILDDIR)/gui_dashboard.o: $(SRCDIR)/gui_dashboard.c
	@echo "  CC (GTK)$<"
	@$(CC) $(APP_CFLAGS) -c $< -o $@

$(BUILDDIR)/main.o: $(SRCDIR)/main.c
	@echo "  CC (GTK)$<"
	@$(CC) $(APP_CFLAGS) -c $< -o $@

# Headless Benchmark Tool
benchmark: directories $(CLI_TARGET)

$(CLI_TARGET): $(CORE_OBJS) $(CLI_TEST_OBJ)
	@echo "  LD      $(CLI_TARGET)"
	@$(CC) $(CORE_OBJS) $(CLI_TEST_OBJ) $(CORE_LIBS) -o $(CLI_TARGET)

$(BUILDDIR)/cli_benchmark.o: $(CLI_TEST_SRC)
	@echo "  CC      $<"
	@$(CC) $(CORE_CFLAGS) -c $< -o $@

test: $(CLI_TARGET)
	@echo "\n>>> RUNNING TEST 1: SYNCHRONIZED (MUTEX / SEMAPHORE) <<<"
	@./$(CLI_TARGET)
	@echo "\n>>> RUNNING TEST 2: UNSYNCHRONIZED (RACE CONDITION DEMONSTRATION) <<<"
	@./$(CLI_TARGET) --race

clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILDDIR)/*.o $(BINDIR)/* $(TESTDIR)/*.bin
	@echo "Clean completed."

help:
	@echo "Available build targets:"
	@echo "  make              - Build core engine & CLI benchmark harness"
	@echo "  make benchmark    - Build headless CLI benchmark tool"
	@echo "  make test         - Run automated test suite (Sync vs Race)"
	@echo "  make gui          - Build Native GTK3 Workstation GUI (Part 2)"
	@echo "  make clean        - Remove all object files and binaries"

