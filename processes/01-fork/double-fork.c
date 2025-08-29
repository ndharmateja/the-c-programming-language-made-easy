#include <stdio.h>
#include <unistd.h>

int main()
{
    // The first fork will create two processes
    // The each of the above 2 processes will run the second fork
    // thereby creating a total of 4 processes
    // all of which will print "Hello world"
    fork();
    fork();
    printf("Hello world!\n");
    return 0;
}