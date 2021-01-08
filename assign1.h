#define TRUE 1
#define FALSE 0
#define CHUNK_SIZE 64000

typedef struct Header
{
	int size;
    int isFree;
    intptr_t *block;
	struct Header *next;
} Header;

/* */
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