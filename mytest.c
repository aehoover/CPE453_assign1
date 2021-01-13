#include <stdio.h>
#include <unistd.h>
#include "malloc.h"

#define startms "Program Start\n"

int main( int argc, char *argv[] )
{
    write( STDOUT_FILENO, startms, sizeof( startms ) );
    //fputs( startms, stdout );

    int *intPtr = ( int* )malloc( (size_t)sizeof( int ) );
    *intPtr = 9;

    int *intPtr2 = ( int* )malloc( ( size_t )sizeof( int ) );
    *intPtr2 = 10;

    int *intPtr3 = ( int* )malloc( ( size_t )sizeof( int ) );
    *intPtr3 = 11;

    /*printf( "%d\n", *intPtr );
    printf( "%d\n",  intPtr );
    printf( "%d\n", *intPtr2 );
    printf( "%d\n",  intPtr2 );
    printf( "%d\n", *intPtr3 );
    printf( "%d\n",  intPtr3 );

    printLinked();*/

    free( intPtr );
    free( intPtr2 );
    free( intPtr3 );

    /*printf( "////////////////\n" );

    printLinked();*/

    return 0;
}