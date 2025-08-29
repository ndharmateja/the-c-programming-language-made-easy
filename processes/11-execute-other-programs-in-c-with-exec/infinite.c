#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main()
{
    printf("program started\n");

    // We are going to run this program itself through exec
    // => infinite loop
    if (execl(
            "./a.out",
            "./a.out",
            NULL) == -1)
    {
        printf("execlp failed\n");
    }

    int errno_val = errno;
    printf("errno: %d\n", errno_val);
    printf("line after execl\n");
    return 0;
}