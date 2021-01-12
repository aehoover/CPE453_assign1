#include <stdio.h>
#include <unistd.h>
#include "malloc.h"

#define startms "Program Start\n"

int main( int argc, char *argv[] )
{
    write( STDOUT_FILENO, startms, sizeof( startms ) );
    //fputs( startms, stdout );

    int *intPtr = ( int* )my_malloc( (size_t)sizeof( int ) );
    *intPtr = 9;

    printf( "%d\n", *intPtr );
    printf( "%d\n",  &( *intPtr ) );

    printLinked();

    my_free( intPtr );

    return 0;
}