UNAME := $(shell uname -m)
ifeq ($(UNAME), x86_64)
CC = arm-linux-gnueabi-gcc
else
CC=gcc
endif

BINS = sds-test asm

all: sds-test asm

asm : asm.S sds.c
	$(CC) -fsanitize=address   -ggdb -O0  $^ -o $@ # -static

sds-test: sds.c sds.h testhelp.h
	$(CC) -o sds-test sds.c -Wall -std=c99 -pedantic -O2 -DSDS_TEST_MAIN
	@echo ">>> Type ./sds-test to run the sds.c unit tests."

clean:
	rm -f $(BINS)
