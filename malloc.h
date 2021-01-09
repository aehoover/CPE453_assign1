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

void *initialize()
{
    return NULL;
}

/* memChunkSetup grabs a CHUNK_SIZE block of memory
by calling sbrk and sets it up for use by malloc. */
void *memChunkSetup();

/* Makes a header for a chunk of memory and places
it in the appropriate space in memory, as retrieved
by memChunkSetup. */
Header *makeHeader( int size, int isFree );

/* */
void *calloc( size_t nmemb, size_t size );

/* malloc takes the size of memory to be allocated, in 
bytes, and returns a pointer to the chunck of memory */
void *malloc( size_t size );

/* */
void free( void *ptr );

/* */
void *realloc( void *ptr, size_t size );