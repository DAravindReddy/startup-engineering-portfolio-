CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -I./source_code/include
TARGET = msh
SRCS = source_code/src/main.c \
       source_code/src/check_command_type.c \
       source_code/src/execute_external_command.c \
       source_code/src/internal_command.c \
       source_code/src/n_pipe.c \
       source_code/src/signal_handler.c

OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
