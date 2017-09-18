CC=gcc
CFLAGS=-std=gnu99 -Wall -Werror -pedantic
CFLAGS+=-g

all: step1 step2 step3

step1: step1.c
	$(CC) $(CFLAGS) -o $@ $<

step2: step2.c
	$(CC) $(CFLAGS) -o $@ $<

step3: step3.c
	$(CC) $(CFLAGS) -o $@ $<

step3_test2: step3_test2.c step3.c
	$(CC) $(CFLAGS) -o $@ $<
