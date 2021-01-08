#include <unistd.h>
#include <stdint.h>
#include "assign1.h"

int main( int argc, char *argv[] )
{
    int headerSize = sizeof( Header );
    int allocSize = ( headerSize + CHUNK_SIZE );
    intptr_t currBreak;

    /* Set up the first chunk of memory */
    Header chunk1;
    chunk1.size = CHUNK_SIZE;
    chunk1.isFree = TRUE;
    chunk1.block = sbrk( 0 );
    chunk1.next = NULL;

    currBreak = sbrk( CHUNK_SIZE );

    Header *makeHeader( int size, int isFree )
    {
        
    }

    void *malloc( size_t size )
    {

    }


    return 0;
}