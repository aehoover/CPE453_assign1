CLAGS = -g -Wall -std=c99 -ansi -pedantic

assign1: assign1.o
	gcc -o assign1 $(CFLAGS) assign1.o

malloc.o: malloc.c malloc.h
	gcc -c $(CFLAGS) malloc.c

assign1.o: assign1.c
	gcc -c $(CFLAGS) assign1.c

clean:
	rm *.o