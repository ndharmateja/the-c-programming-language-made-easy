#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    // Fork a child process
    int fork_id1 = fork();
    if (fork_id1 == -1)
    {
        printf("Fork failed.\n");
        return 1;
    }

    // Child process 1
    if (fork_id1 == 0)
    {
        sleep(4);
        printf("[c1](pid: %d) finished sleeping\n", getpid());
        return 0;
    }

    // Fork another child process
    int fork_id2 = fork();
    if (fork_id2 == -1)
    {
        printf("Fork failed.\n");
        return 1;
    }

    // Child process 2
    if (fork_id2 == 0)
    {
        sleep(2);
        printf("[c2](pid: %d) finished sleeping\n", getpid());
        return 0;
    }

    // Parent process
    // Wait for specific child processes to finish
    waitpid(fork_id1, NULL, 0);
    printf("[p] finished waiting for child %d\n", fork_id1);
    waitpid(fork_id2, NULL, 0);
    printf("[p] finished waiting for child %d\n", fork_id2);

    // Exit
    printf("[p](pid: %d) finished execution\n", getpid());
    return 0;
}