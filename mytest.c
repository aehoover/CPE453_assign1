#include <stdio.h>
#include <unistd.h>
#include "malloc.h"

#define startms "Program Start\n"

int main( int argc, char *argv[] )
{
    write( STDERR_FILENO, startms, sizeof( startms ) );
    //fputs( startms, stdout );

     int *intPtr = ( int* )malloc( (size_t)sizeof( int ) );
    *intPtr = 9;

    int *intPtr2 = ( int* )malloc( ( size_t )sizeof( int ) );
    *intPtr2 = 10;

    int *intPtr3 = ( int* )malloc( ( size_t )sizeof( int ) );
    *intPtr3 = 11;

    //printLinked();
    //printf( "//////////////////\n" );

    //free( intPtr );
    free( intPtr2 );
    //free( intPtr3 ); 

    intPtr = realloc( NULL, ( size_t )sizeof( int ) );
    *intPtr = 3;
    //printf( "intPtr: %ld\n", ( intptr_t )intPtr );
    //printLinked();
    

    //int *intPtr4 = realloc( NULL, ( size_t )sizeof( int ) );


    return 0;
}