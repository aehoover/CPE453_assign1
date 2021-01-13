#define TRUE 1
#define FALSE 0
#define CHUNK_SIZE 64000

typedef struct Header
{
	int size;
    int isFree;
    void *block;
	struct Header *next;
} Header;

/* copyMem takes two pointers to headers and copies the
contents of oldH's memory locations to those of newH */
void copyMem( Header *oldH, Header *newH );

/* */
void printLinked();

/* rndUpNrst16 takes a number of bytes and checks
if it is evenly divisible by 16. If it is not, it
rounds bytes up to the nearest number that is, and
returns that number. */
int rndUpNrst16( int bytes );

/* */
void *movePtr( void *ptr, int bytes );

/* Takes a memory address and a number of bytes. 
Initializes that number of bytes of memory, starting
at the location given, to zero */
void setToZero( void *ptr, int numBytes );

/* merge takes two pointers, each to headers of adjacent
memory chunks in the list. It then merges the two chunks
into a single chunk. */
void merge( Header *l, Header *r );

/* deFragment iterates through the list of memory chunks
and merges adjacet chunks into a single chunk if they are
both free. */
void deFragment( Header *header );

/* extendChunk takes the address of the header of
a chunk of free memory. If the header is the tail
of the memory list, it uses sbrk to extend the
chunk of memory by CHUNK_SIZE bytes */
void extendChunk( Header *header );

/* tail takes a pointer to the first header in the
list of memory. It iterates through the list until
it finds the tail and returns a pointer to that
header. */
Header *getTail( Header *listHead );

/* allignBlock takes the location of the header of
a free block of memory. It then checks that the starting
address of the free memory is evenly divisible by 16, 
and if it is not, it moves space by space to find the 
first available address that is. It returns nothing. */
void allignBlock( Header *address );

/* isAlligned takes a memory address and returns
1 if it is divisible by 16 and 0 otherwise */
int isAlligned( Header *address );

/* findFreeMem searches the list of memory chunks set
aside for the program. It returns a pointer to the 
header of the free space if found, and NULL otherwise. */
Header *findFreeMem( int size );

/* location takes a pointer to a header of a chunk of 
memory and calculates the location of the usable 
memory */
void *location( Header *headerLocation );

/* memChunkSetup grabs a CHUNK_SIZE block of memory
by calling sbrk and sets it up for use by malloc. It
returns the address of the header at the start of 
the chunk. */
void *memChunkSetup();

/* */
void *calloc( size_t nmemb, size_t size );

/* malloc takes the size of memory to be allocated, in 
bytes, and returns a pointer to the chunck of memory */
void *malloc( size_t size );

/* */
void free( void *ptr );


/* */
void *realloc( void *ptr, size_t size );

void *realloc2( void *ptr, size_t size );