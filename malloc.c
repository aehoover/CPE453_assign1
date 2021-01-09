#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include "malloc.h"

static int initialized = FALSE;

void *initialize();

void *memChunkSetup()
{
    int headerSize = sizeof( Header );
    Header *currBreak = sbrk( 0 );
    /* Grab a chunk of memory for use by the program
    by calling sbrk. */
    void *newBreak = sbrk( headerSize + CHUNK_SIZE );

    /* Insert a header for the chunk of memory in
    front of the chunk. */
    Header *newHeader = makeHeader( CHUNK_SIZE, TRUE, currBreak,
    NULL );

    return newHeader;
}

Header *makeHeader( int size, int isFree, void *insertLocation,
void *next )
{
    Header *newHeader;
    newHeader = currBreak;
    newHeader->size = size;
    newHeader->isFree = isFree;
    newHeader->block = insertLocation;
    newHeader->next = next;

    return newHeader;
}

void *calloc( size_t nmemb, size_t size )
{

    return NULL;

}

void *malloc( size_t size )
{
    if ( !initialized )
    {
        /* Set up the first chunck of memory
        for use by the program */
    }
    else
    {
        /* Look for free space in memory that has
        already been set aside for the program */
    }

    return NULL;

}

void free( void *ptr )
{

}

void *realloc( void *ptr, size_t size )
{
    return NULL;

}