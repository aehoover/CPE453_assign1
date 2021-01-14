#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "malloc.h"

#define msg "In my malloc\n"
#define freemsg "In my free\n"
#define nullFree "Null ptr has been freed\n"
#define realmsg "In my realloc\n"
#define callmsg "In my calloc\n"


static void *programMem = NULL;
static int HEADER_SIZE;

void fillWithZeros( Header *ptr, int bytes )
{
    int i;
    char *data;
    
    if ( ptr != NULL )
    {
        data = ptr->block; /* Start of the bytes we
                             will fill with zeros */
        
        for ( i = 0; i < bytes; i++ )
        {
            *data = 0;
            data += 1;
        }
    }  
}

void copyMem( Header *oldH, Header *newH )
{
    int i;
    int size = oldH->size;
    char *oldMemPtr = oldH->block;
    char *newMemPtr = newH->block;

    for ( i = 0; i < size; i++ )
    {
        /* Copy contents of the old memory space to the
        new one */
        *newMemPtr = *oldMemPtr;

        oldMemPtr += 1;
        newMemPtr += 1;
    }
}

void printLinked()
{
    Header *head = programMem;

    while ( TRUE )
    {
        printf( "header start: %ld\n", ( intptr_t )head );
        printf( "block start: %ld\n", ( intptr_t )head->block );
        printf( "status: %d\n", head->isFree );
        
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
    l->size += ( r->size + HEADER_SIZE );

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
        //void *currBreak = sbrk( ( int )CHUNK_SIZE );
        sbrk( ( int )CHUNK_SIZE );
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
        if ( head->isFree == TRUE && head->size >= 
             size + HEADER_SIZE )
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

    if ( currBreak == ( void* )-1 )
    {
        perror( "Error, failed to move break no memory\n" );
    }

    /* Round the size of the header up to the
    nearest 16, to ensure proper alignment */
    headerSize = rndUpNrst16( headerSize );

    /* Set the size of headers to be used for all
    calls to malloc, so we do not need to keep 
    calculating it */
    HEADER_SIZE = headerSize;

    /* Grab a chunk of memory for use by the program
    by calling sbrk. */
    void *newBreak = sbrk( headerSize + CHUNK_SIZE );

    if ( newBreak == ( void* )-1 )
    {
        perror( "Error, failed to move break no memory\n ");
    }

    /* Insert a header for the chunk of memory in
    front of the chunk. */
    Header *newHeader = ( Header* )currBreak;
    newHeader->size = CHUNK_SIZE;
    newHeader->isFree = FALSE;
    newHeader->block = ( char* )newHeader + headerSize;
    newHeader->next = NULL;

    /* Make sure the free space starts on an
    address that is divisible by 16 */
    allignBlock( newHeader );

    return newHeader;
}

void *calloc( size_t nmemb, size_t size )
{
    //write( STDOUT_FILENO, callmsg, sizeof( callmsg ) );

    /* TO DO: Find way to test in the calculated 
    size will result in an intever overflow */
    size_t bytes = ( nmemb * size );
    
    Header *location = malloc( bytes );

    if ( location != NULL )
    {
        /* Initialize the allocated memory to all
        zeros */
        fillWithZeros( location->block, bytes );
    }

    return location;
}

void *malloc( size_t size )
{
    Header *toAllocate = NULL;
    Header *nextHeader = NULL;

    write( STDERR_FILENO, msg, sizeof( msg ) );
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
                toAllocate = memChunkSetup();

                /* Check to make sure the chunk size we
                are using is big enough */
                if ( CHUNK_SIZE <= size )
                {
                    while ( toAllocate->size <= size )
                    {
                        Header *secondChunk = memChunkSetup();
                        toAllocate->next = secondChunk;
                        merge( toAllocate, secondChunk );
                    }
                }

                /* Add the new tail to the end of the 
                memory list */
                tail->next = toAllocate;
            }
    }

    /* Put another header after the memory the user
    requested, to keep track of how much free space
    is available to the program */
    //nextHeader = movePtr( toAllocate->block, size );
    nextHeader = ( Header* )( ( char* )toAllocate->block +
                                       size );
    nextHeader->size = ( toAllocate->size - size -
                       ( 2 * HEADER_SIZE ) );
    nextHeader->isFree = TRUE;
    nextHeader->block = ( char* )nextHeader + HEADER_SIZE;
    nextHeader->next = NULL;

    /* Finish setting up the header of the block 
    that will be allocated */
    toAllocate->size = size;
    toAllocate->isFree = FALSE;
    toAllocate->next = nextHeader;
    
    return toAllocate->block;
}

void free( void *ptr )
{
    Header *head = programMem;

    write( STDOUT_FILENO, freemsg, sizeof( freemsg ) );
    //fputs( freemsg, stdout );

    if ( ptr == NULL )
    {
        //write( STDOUT_FILENO, nullFree, sizeof( nullFree ) );
    }
    else
    {
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
}

void *realloc( void *ptr, size_t size )
{
    write( STDOUT_FILENO, realmsg, sizeof( realmsg ) );

    Header *newLocation = NULL;
    Header *hPtr = ptr;

    if ( hPtr == NULL )
    {
        /* If realloc is called with NULL it does the 
        same thing as malloc */

        newLocation = malloc( size );
    }
    else if ( size == 0 )
    {
        /* If realloc is called with a non NULL ptr and
        a size of zero, it frees that location */
        free( hPtr );
        return NULL;
    }
    else
    {
        /* We need to shrink or expand the memory space,
        or copy to a new one if that is not possible */

        /* If the block size is more than we want, we
        shrink the block */
        if ( hPtr->size > size )
        {
            newLocation = hPtr;

            /* Calculate how much space will be left
            after shrinking */
            int leftoverChunk = hPtr->size - size;

            if ( leftoverChunk >= HEADER_SIZE )
            {
                /* Make new header for leftover space
                and put it into the correct space in the 
                list */
                Header *newH = ( Header* )( ( char* )hPtr->block + size );
                newH->size = leftoverChunk;
                newH->isFree = TRUE;
                newH->next = hPtr->next;

                newLocation->next = newH;
            }
            else
            {
                /* Shrinking not possible, find a new
                location and copy */
                newLocation = findFreeMem( size );
                copyMem( hPtr, newLocation );
                
                /* Free up the space that was copied
                from */
                free( hPtr->block );
            }
        }
        else if ( hPtr->size < size )
        {
            /* If we need more space, we try to 
            expand */

            /* Check if the adjacent space is free */
            if ( hPtr->next != NULL &&
                 hPtr->next->isFree == TRUE )
            {
                /* Check if it would be big enough */
                int spaceAvail = ( HEADER_SIZE + 
                                   hPtr->next->size + 
                                   hPtr->size );
                /* Check that any leftover space is big
                enough that it can be tracked with a
                header */
                if ( spaceAvail >= size )
                {
                    int leftoverChunk = spaceAvail - size;

                    if ( leftoverChunk < HEADER_SIZE )
                    {
                        /* Unable to expand and must
                        find a new location and copy */

                        newLocation = findFreeMem( size );
                        copyMem( hPtr, newLocation );
                
                        /* Free up the space that was copied
                        from */
                        free( hPtr->block ); 
                    }
                    else
                    {
                        /* Do the expansion */
                        newLocation = hPtr;
                        merge( hPtr, hPtr->next );

                        /* New header to put on any 
                        leftover free space */
                        Header *newH = ( Header* )( ( char* )hPtr->block + size );
                        newH->size = hPtr->size - size;
                        newH->isFree = TRUE;
                        newH->next = hPtr->next;

                        /* Adjust the realloced header
                        and link it in front of the 
                        new one */
                        newLocation->size = size;
                        newLocation->next = newH;
                    }
                }
            }
            else
            {
                /* We can't expand, find a new 
                location and copy */
                newLocation = findFreeMem( size );
                copyMem( hPtr, newLocation );
                
                /* Free up the space that was copied
                from */
                free( hPtr->block );
            }
        }
        else
        {
            /* If the size of the space pointed to is
            the same size as requested, we will free
            the space and return it */

            newLocation = hPtr;
            newLocation->isFree = TRUE;
        }
    }

    return newLocation->block;
}