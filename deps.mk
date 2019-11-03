CC = gcc
CFLAGS = -g -O2 -std=c99 -Wall -Werror -pedantic -Wformat=2 -Wshadow -Wpointer-arith -Wconversion -Wunreachable-code -Wno-sign-conversion -Wbad-function-cast -Wtype-limits
VAL: valgrind --leak-check=full --track-origins=yes --show-reachable=yes
heap: heap.c heap.h main.c pruebas_alumno.c testing.c testing.h
	$(CC) $(CFLAGS) -o heap *.c

val: heap
	$(VAL) ./heap


