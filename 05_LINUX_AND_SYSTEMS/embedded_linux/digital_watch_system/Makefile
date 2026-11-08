# Makefile for Embedded Linux Digital Watch System
# Supports native compilation and cross-compilation (e.g., CROSS_COMPILE=arm-linux-gnueabihf-)

CC ?= $(CROSS_COMPILE)gcc
CFLAGS ?= -Wall -Wextra -Werror -pedantic -std=c11 -O2 -Iinclude
LDFLAGS ?= 

SRC = src/watch_core.c src/display.c src/main.c
OBJ = $(SRC:.c=.o)
TARGET = digital_watch

TEST_SRC = src/watch_core.c tests/test_watch_core.c
TEST_TARGET = test_runner

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test: $(TEST_SRC)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $^ $(LDFLAGS)
	./$(TEST_TARGET)
	rm -f $(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) src/*.o
