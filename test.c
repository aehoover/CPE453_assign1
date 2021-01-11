#include <stdio.h>
#include "malloc.h"

int main( int argc, char *argv[] )
{
    printf( "Starting program to test malloc\n" );

    int *intPtr = ( int* )malloc( (size_t)sizeof( int ) );
    *intPtr = 9;

    return 0;
}