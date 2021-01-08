CC = gcc
CFLAGS = -std=c99 -Wall -ansi -pedantic -g
MAIN = assign1
OBJS = assign1.o
all : $(MAIN)

$(MAIN) : $(OBJS) assign1.c
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

assign1.o : assign1.c assign1.h
	$(CC) $(CFLAGS) -c assign1.c

clean :
	rm *.o $(MAIN) core