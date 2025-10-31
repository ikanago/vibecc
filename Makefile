CC := gcc
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Werror -g -O0

SRC_DIR := src
BIN_DIR := bin

INCLUDES := -I$(SRC_DIR)
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
TARGET := $(BIN_DIR)/vibecc

.PHONY: all
all: $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)

.PHONY: format
format:
	clang-format -i $(SRC_DIR)/*.c $(SRC_DIR)/*.h

.PHONY: run
run: $(TARGET)
	$(TARGET)

.PHONY: test
test: $(TARGET)
	bash tests.sh
