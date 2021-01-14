CC = gcc
CFLAGS = -g -Wall -pedantic -fPIC

malloc: libmalloc.a libmalloc.so
	echo done

malloc.o: malloc.c malloc.h
	$(CC) $(CFLAGS) -c -fPIC malloc.c

mytest.o: mytest.c malloc.h
	$(CC) $(CFLAGS) -c mytest.c

libmalloc.a: malloc.o
	ar r libmalloc.a malloc.o

libmalloc.so: malloc.o
	$(CC) $(CFLAGS) -shared -fPIC -o libmalloc.so malloc.o

mytest: mytest.o malloc.o
	$(CC) $(CFLAGS) mytest.o -L. -lmalloc

.PHONY: malloc

intel-all: lib/libmalloc.so lib64/libmalloc.so

lib/libmalloc.so: lib malloc32.o
	$(CC) $(CFLAGS) -m32 -shared -o $@ malloc32.o
	
lib64/libmalloc.so: lib64 malloc64.o
	$(CC) $(CFLAGS) -shared -o $@ malloc64.o

lib:
	mkdir lib
	
lib64:
	mkdir lib64
	
malloc32.o: malloc.c
	$(CC) $(CFLAGS) -m32 -c -o malloc32.o malloc.c

malloc64.o: malloc.c
	$(CC) $(CFLAGS) -m64 -c -o malloc64.o malloc.c

clean:
	rm *.o *.a *.so *.out