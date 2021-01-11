CC = gcc
CLAGS = -g -Wall -std=c99 -ansi -pedantic

malloc: libmalloc.a libmalloc.so

.PHONY: malloc

malloc.o: malloc.c malloc.h
	gcc -c $(CFLAGS) malloc.c

test.o: test.c malloc.h
	gcc $(CFLAGS) -c test.c

libmalloc.a: malloc.o
	ar r libmalloc.a malloc.o

libmalloc.so: malloc.o
	$(CC) -shared -fPIC -o libmalloc.so malloc.o

test: test.o malloc.o
	gcc $(CFLAGS) -o test test.o

clean:
	rm *.o *.a *.so