#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    // Fork a child process
    int x = 2;
    int fork_id = fork();
    if (fork_id == -1)
    {
        printf("Fork failed.\n");
        return 1;
    }

    // Increment x only in the child process
    if (fork_id == 0)
        x++;

    // The print statement will be executed by
    // both the child and the parent processes
    // x's value is different in each process as each process
    // has its own memory and its own copy of x
    printf("[%d] Hello from processes! x: %d\n", getpid(), x);

    // Wait for the child process to finish
    if (fork_id != 0)
        wait(NULL);

    return 0;
}