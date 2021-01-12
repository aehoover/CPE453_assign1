CC = gcc
CFLAGS = -g -Wall -pedantic

malloc: libmalloc.a libmalloc.so
	echo done

malloc.o: malloc.c malloc.h
	$(CC) $(CFLAGS) -c -fPIC malloc.c

test.o: test.c malloc.h
	$(CC) $(CFLAGS) -c mytest.c

libmalloc.a: malloc.o
	ar r libmalloc.a malloc.o

libmalloc.so: malloc.o
	$(CC) $(CFLAGS) -shared -fPIC -o libmalloc.so malloc.o

mytest: mytest.o malloc.o
	$(CC) $(CFLAGS) mytest.o -L. -lmalloc

.PHONY: malloc

clean:
	rm *.o *.a *.so *.out