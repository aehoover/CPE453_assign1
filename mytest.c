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

    //free( intPtr );
    free( intPtr2 );
    //free( intPtr3 ); 

    //int *intPtr4 = realloc( NULL, ( size_t )sizeof( int ) );


    return 0;
}