#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// Exec family of functions
// l => List — Pass the arguments as a comma - separated list ending with NULL.
// v => Vector — Pass the arguments as an array of strings.
// p => Path — Pass the arguments as a NULL - terminated array of strings.
// e => Environment — Pass the environment as an array of strings.
// execv
// execvp
// execve
// execvpe
// execl
// execlp
// execle
// execlpe

int main()
{
    if (execlp("ping", "ping", "-c", "3", "www.google.com", NULL) == -1)
    {
        printf("execlp failed\n");
    }

    int errno_val = errno;
    printf("errno: %d\n", errno_val);
    printf("line after execl\n");
    return 0;
}