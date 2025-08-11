#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main()
{
    // Fork a child process
    int fork_id = fork();
    if (fork_id == -1)
    {
        printf("Fork failed.\n");
        return 1;
    }

    // Run the ping command in the child process
    if (fork_id == 0)
    {
        if (execlp("ping", "ping", "-c", "3", "www.google.com", NULL) == -1)
        {
            printf("execlp failed\n");
            int errno_val = errno;
            printf("errno: %d\n", errno_val);
            printf("line after execl\n");
            return 2;
        }

        printf("This shouldn't be printed\n");
    }

    // Wait for the child process to finish
    wait(NULL);
    printf("\nFirst ping command finished\n\n");

    // Fork another child process
    int fork_id2 = fork();
    if (fork_id2 == -1)
    {
        printf("Fork failed.\n");
        return 1;
    }

    // Run the same ping command in the second child process
    // but with execvp
    if (fork_id2 == 0)
    {
        char *args[] = {"ping", "-c", "3", "www.google.com", NULL};
        if (execvp("ping", args) == -1)
        {
            printf("execvp failed\n");
            int errno_val = errno;
            printf("errno: %d\n", errno_val);
            printf("line after execl\n");
            return 3;
        }

        printf("This shouldn't be printed\n");
    }

    // Wait for the second child process to finish
    wait(NULL);
    printf("\nSecond ping command finished\n");
    printf("Program finished\n");
    return 0;
}