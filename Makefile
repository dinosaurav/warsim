CC=gcc
CFLAGS=-Wall -Iincludes -std=c99 -ggdb

all: warsim

warsim: warsim.c warsim.h
	$(CC) $(CFLAGS) warsim.c -o warsim

clean:
	rm warsim

test: warsim
	./warsim -n 1
