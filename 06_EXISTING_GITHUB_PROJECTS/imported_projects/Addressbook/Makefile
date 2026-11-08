CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I./source_code/include
TARGET = addressbook
SRCS = source_code/src/contact.c \
       source_code/src/dummy_contact.c \
       source_code/src/main.c

OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe
