#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include "malloc.h"

static void *programMem = NULL;

void extendChunk( Header *header )
{
    Header *tail = getTail( programMem );

    if ( header == tail )
    {
        void *currBreak = sbrk( CHUNK_SIZE );
    }
}

Header *tail( Header *listHead )
{
    Header *head = listHead;

    while ( head->next != NULL )
    {
        head = head->next;
    }

    return head;
}

void allignBlock( Header *address )
{
    while ( isAlligned( address->block ) == FALSE )
    {
        /* move to the next available address */
        address->block += 1;
        /* decrement the size of the free memory
        by one location */
        address->size -= 1; 
    }
}

int isAlligned( Header *address )
{
    int allignment = FALSE;

    if ( ( intptr_t )address % 16 == 0 )
    {
        allignment = TRUE;
    }

    return allignment;
}

Header *findFreeMem( int size )
{
    Header *head = programMem;
    Header *location = NULL;

    while ( TRUE )
    {
        if ( head->isFree == TRUE && head->size >= size )
        {
            location = head;
            break;
        }
        else if ( head->next == NULL )
        {
            break;
        }

        head = head->next;
    }

    return location;
}

void *location( Header *headerLocation )
{
    void *location = headerLocation + sizeof( Header );

    /* Adjust the starting address of the free memory
    to be divisible by 16 if neccessary */
    allignBlock( location );

    return location;
}

void *memChunkSetup()
{
    int headerSize = sizeof( Header );
    Header *currBreak = sbrk( 0 );
    /* Grab a chunk of memory for use by the program
    by calling sbrk. */
    void *newBreak = sbrk( headerSize + CHUNK_SIZE );

    /* Insert a header for the chunk of memory in
    front of the chunk. */
    Header *newHeader = currBreak;
    newHeader->size = CHUNK_SIZE;
    newHeader->isFree = TRUE;
    newHeader->block = location( newHeader );
    newHeader->next = NULL;

    /* Make sure the free space starts on an
    address that is divisible by 16 */
    allignBlock( newHeader );

    return newHeader;
}

void *calloc( size_t nmemb, size_t size )
{

    return NULL;

}

void *malloc( size_t size )
{
    Header *toAllocate = NULL;

    /* If malloc has not previously been called, 
    initialize the first chunk of memory. */
    if ( programMem == NULL )
    {
        Header *memHeader = memChunkSetup();
        programMem = memHeader;
        toAllocate = memHeader;
    }
    else
    {
        /* Search the current list of chunks for free
        memory to give the user */
        toAllocate = findFreeMem( size );

            /* If no free memory large enough is found, use sbrk
            to grab another chunk of memory */
            if ( toAllocate == NULL )
            {
                Header *tail = getTail( programMem );
                tail = memChunkSetup();
                toAllocate = tail;
            }
    }

    /* Put another header after the memory the user
    requested, to keep track of how much free space
    is available to the program */
    Header *nextHeader = toAllocate->block + size;
    nextHeader->size = ( toAllocate->size - size -
                       ( sizeof( Header ) ) );
    nextHeader->isFree = TRUE;
    nextHeader->block = location( nextHeader );
    nextHeader->next = NULL;

    return toAllocate;
}

void free( void *ptr )
{

}

void *realloc( void *ptr, size_t size )
{
    return NULL;

}