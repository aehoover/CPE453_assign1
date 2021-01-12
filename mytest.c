#include <stdio.h>
#include <unistd.h>
#include "malloc.h"

#define startms "Program Start\n"

int main( int argc, char *argv[] )
{
    write( STDOUT_FILENO, startms, sizeof( startms ) );

    int *intPtr = ( int* )malloc( (size_t)sizeof( int ) );
    *intPtr = 9;

    return 0;
}