CC=gcc
CFLAGS=-Wall -pthread

makeit: hw3.c
	$(CC) hw3.c -o hw3 $(CFLAGS)
	
