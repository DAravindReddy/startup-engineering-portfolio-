# Kernel Module Makefile for kernelwise_char driver
KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m += src/kernelwise_char.o

CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2 -Iinclude

TEST_APP = test_user_app

.PHONY: all modules user_app clean install load unload test

all: modules user_app

modules:
	@if [ -d "$(KDIR)" ]; then \
		$(MAKE) -C $(KDIR) M=$(PWD) modules; \
	else \
		echo "[NOTICE] Kernel headers not found at $(KDIR). Skipped kernel module build on non-Linux host."; \
	fi

user_app: tests/test_user_app.c
	$(CC) $(CFLAGS) -o $(TEST_APP) $<

load:
	sudo insmod src/kernelwise_char.ko
	sudo chmod 666 /dev/kernelwise_char

unload:
	sudo rmmod kernelwise_char

test: user_app
	./$(TEST_APP)

clean:
	@if [ -d "$(KDIR)" ]; then \
		$(MAKE) -C $(KDIR) M=$(PWD) clean; \
	fi
	rm -f $(TEST_APP) src/*.o src/*.ko src/*.mod* src/.*.cmd src/Module.symvers src/modules.order
