CFLAGS = -O1 -lrt -g -o
CC = gcc

all: test

test: test.c
	$(CC) $? $(CFLAGS) $@
	touch buf_test/test.buf

clean: test
	rm -rf $? buf_test/test.buf
