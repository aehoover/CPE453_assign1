#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "malloc.h"

#define msg "In my malloc\n"
#define freemsg "In my free\n"
#define nullFree "Null ptr has been freed\n"

static void *programMem = NULL;

void printLinked()
{
    Header *head = programMem;

    while ( TRUE )
    {
        printf( "header start: %d\n", head );
        printf( "block start: %d\n", head->block );
        
        if ( head->next == NULL )
        {
            break;
        }

        head = head->next;
    }
}

int rndUpNrst16( int bytes )
{
    while ( bytes % 16 != 0 )
    {
        bytes += 1;
    }

    return bytes;
}

void *movePtr( void *ptr, int bytes )
{
    intptr_t ptrInt = ( intptr_t )ptr;

    ptrInt += bytes;

    return ( void* )ptrInt;
}

void setToZero( void *ptr, int numBytes )
{

}

void merge( Header *l, Header *r )
{
    /* Calculate the size of the new chunk and update
    the l header's size */
    l->size += ( r->size + sizeof( Header ) );

    /* unlink the right chunk from the list */
    if ( r->next == NULL )
    {
        l->next = NULL;
    }
    else
    {
        l->next = r->next;
    }
}

void deFragment( Header *headPtr )
{
    Header *head = headPtr;

    while( head->next != NULL )
    {
        if ( head->isFree == TRUE && 
             head->next->isFree == TRUE )
        {
            /* merge the chunks */
            merge( head, head->next );
        }
        else
        {
            head = head->next;
        }
    }
}

void extendChunk( Header *header )
{
    Header *tail = getTail( programMem );

    if ( header == tail )
    {
        void *currBreak = sbrk( ( int )CHUNK_SIZE );
        tail->size += CHUNK_SIZE;
    }
}

Header *getTail( Header *listHead )
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
        address->block = movePtr( address->block, 1 );
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
    void *currBreak = sbrk( 0 );

    /* Round the size of the header up to the
    nearest 16, to ensure proper alignment */
    headerSize = rndUpNrst16( headerSize );

    /* Grab a chunk of memory for use by the program
    by calling sbrk. */
    void *newBreak = sbrk( headerSize + CHUNK_SIZE );

    /* Insert a header for the chunk of memory in
    front of the chunk. */
    Header *newHeader = ( Header* )currBreak;
    newHeader->size = CHUNK_SIZE;
    newHeader->isFree = TRUE;
    newHeader->block = ( char* )newHeader + headerSize;
    newHeader->next = NULL;

    /* Make sure the free space starts on an
    address that is divisible by 16 */
    //allignBlock( newHeader );

    return newHeader;
}

void *my_calloc( size_t nmemb, size_t size )
{
    /* TO DO: Find way to test in the calculated 
    size will result in an intever overflow */
    size_t bytes = ( nmemb * size );
    
    Header *location = my_malloc( bytes );

    if ( location != NULL )
    {
        /* Initialize the allocated memory to all
        zeros */
    }

    return location;

}

void *my_malloc( size_t size )
{
    Header *toAllocate = NULL;
    Header *nextHeader = NULL;

    

    write( STDOUT_FILENO, msg, sizeof( msg ) );
    //fputs( msg, stdout );

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
    nextHeader = movePtr( toAllocate->block, size );
    nextHeader->size = ( toAllocate->size - size -
                       ( sizeof( Header ) ) );
    nextHeader->isFree = TRUE;
    nextHeader->block = location( nextHeader );
    nextHeader->next = NULL;

    /* Link the memory the program requested to the 
    free memory behind it */
    toAllocate->next = nextHeader;

    return toAllocate->block;
}

void my_free( void *ptr )
{
    Header *head = programMem;

    write( STDOUT_FILENO, freemsg, sizeof( freemsg ) );
    //fputs( freemsg, stdout );

    if ( ptr == NULL )
    {
        write( STDOUT_FILENO, nullFree, sizeof( nullFree ) );
    }


    while ( TRUE )
    {
        /* Find the block in the list and 
        free it */
        if ( head->block == ptr )
        {
            head->isFree = TRUE;
            break;
        }
        else if ( head->next == NULL )
        {
            /* otherwise, if the address is not in our
            list of memory...*/
            perror( "Error, location not found\n" );
            break;
        }

        head = head->next;
    }

    deFragment( programMem );
}


void *my_realloc( void *ptr, size_t size )
{
    Header *newLocation = NULL;
    Header *hPtr = ptr;

    if ( ( hPtr->size < size ) )
    {
        int diff = ( hPtr->size - size );

        if ( diff > sizeof( Header ) )
        {
            Header *newHeader;
            intptr_t ptrLocation = ( intptr_t )hPtr + size;

            /* Keep the location the same */
            newLocation = hPtr;
            /* Update the size of the memory */
            hPtr->size = size;

            /* Insert a new header for the leftover
            memory */
            newHeader = ( Header* )ptrLocation;
            newHeader->size = ( diff - sizeof( Header ) );
            newHeader->isFree = TRUE;
            /* TO DO: Update functions so that if when
            aligning memory addresses to be divisible by
            16, it uses up all available memory in the block
            before aligning, it makes that spot unusable */
            newHeader->block = location( newHeader );
            newHeader->next = hPtr->next;

            hPtr->next = newHeader;
        }
        else
        {
            /* Need to grab a different free space to
            give the user */
            newLocation = findFreeMem( size );

            if ( newLocation == NULL )
            {
                newLocation = my_malloc( size );
            }
        }
    }
    else if ( hPtr->size > size )
    {
        newLocation = findFreeMem( size );

        if ( newLocation == NULL )
        {
            newLocation = my_malloc( size );
        }
    }
    else
    {
        newLocation = hPtr;
    }

    return newLocation;
}
