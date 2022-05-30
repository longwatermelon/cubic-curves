CC=gcc
CFLAGS=-std=gnu17 -ggdb -Wall -Wpedantic -Werror
LIBS=-lm -lSDL2

all: main.c
	$(CC) $(CFLAGS) $< $(LIBS)

