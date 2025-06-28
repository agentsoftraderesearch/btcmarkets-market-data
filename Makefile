# Compiler and flags
CC = gcc
COMMON_CFLAGS = -Wall -Wextra -std=c11 -Iinclude
CFLAGS = $(COMMON_CFLAGS) -fPIC

# Directories
SRC_DIR = src
EXAMPLES_DIR = examples
BUILD_DIR = build
BIN_DIR = bin
LIB_DIR = lib
TEST_DIR = tests/integration_tests
TEST_BIN_DIR = $(TEST_DIR)/bin

# Installation directories (can be overridden)
PREFIX ?= /usr/local
INCLUDE_DIR = include
INCLUDE_INSTALL_DIR = $(PREFIX)/include/btcmarkets
LIB_INSTALL_DIR = $(PREFIX)/lib

# Target executable name
TARGET = $(BIN_DIR)/main

# Source and object files
MAIN_SRC = $(EXAMPLES_DIR)/main.c
ALL_SRCS = $(wildcard $(SRC_DIR)/*.c)
APP_SRCS = $(ALL_SRCS)

# Object files
LIB_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(APP_SRCS))
MAIN_OBJ = $(BUILD_DIR)/main.o

# Shared library
SHARED_LIB = $(LIB_DIR)/libmarket_data.so

# Test files
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_BINS = $(patsubst $(TEST_DIR)/%.c, $(TEST_BIN_DIR)/%, $(TEST_SRCS))

# Default build
all: $(SHARED_LIB) $(TARGET)

# Build the shared library
$(SHARED_LIB): $(LIB_OBJS) | $(LIB_DIR)
	$(CC) $(CFLAGS) -shared -o $@ $^

# Build final executable from main.o + shared library
$(TARGET): $(MAIN_OBJ) $(SHARED_LIB) | $(BIN_DIR)
	$(CC) $(COMMON_CFLAGS) -L$(LIB_DIR) -Wl,-rpath,$(LIB_DIR) -o $@ $^ -lmarket_data -lcurl -lcjson

# Compile library source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile main.o from examples
$(MAIN_OBJ): $(MAIN_SRC) | $(BUILD_DIR)
	$(CC) $(COMMON_CFLAGS) -c -o $@ $<

# Compile test binaries
$(TEST_BIN_DIR)/%: $(TEST_DIR)/%.c $(SHARED_LIB) | $(TEST_BIN_DIR)
	$(CC) $(COMMON_CFLAGS) -L$(LIB_DIR) -Wl,-rpath,$(LIB_DIR) -o $@ $< -lmarket_data -lcriterion -lcurl -lcjson

# Run tests
test: all $(TEST_BINS)
	@for test in $(TEST_BINS); do \
		echo "Running $$test"; \
		./$$test || exit 1; \
	done

# Install headers and shared library
install: $(SHARED_LIB)
	@echo "Installing headers to $(INCLUDE_INSTALL_DIR)"
	install -d $(INCLUDE_INSTALL_DIR)
	install -m 644 $(INCLUDE_DIR)/btcmarkets/*.h $(INCLUDE_INSTALL_DIR)
	@echo "Installing shared library to $(LIB_INSTALL_DIR)"
	install -d $(LIB_INSTALL_DIR)
	install -m 755 $(SHARED_LIB) $(LIB_INSTALL_DIR)

# Ensure required directories exist
$(BUILD_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

$(LIB_DIR):
	mkdir -p $@

$(TEST_BIN_DIR):
	mkdir -p $@

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(LIB_DIR) $(TEST_BIN_DIR)

.PHONY: all clean test install
