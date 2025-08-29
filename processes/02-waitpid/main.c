#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// waitpid is used to wait for a specific child process to finish
// If -1 is passed as pid, it will wait for any child process to finish
// If 0 is passed as pid, it will wait for any child process whose group ID is
// the same as the calling process
// If >0 is passed as pid, it will wait for the child process whose process ID
// is equal to pid
// We can also pass the options WNOHANG etc as the third parameter
// of waitpid, to know if the child process has finished or not but it would
// not wait for the child process to finish if WNOHANG is passed

// wait is used to wait for any child process to finish
// and wait(NULL) returns the process ID of the child process that finished

int main()
{
    // Fork a child process
    int fork_id = fork();
    if (fork_id == -1)
    {
        printf("Fork failed.\n");
        return 1;
    }

    // Child process 1
    if (fork_id == 0)
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
    // Wait for whichever child process finishes first
    // and then wait for the other child process
    int wait_result = wait(NULL);
    printf("[p] Finished waiting for child process %d\n", wait_result);
    wait_result = wait(NULL);
    printf("[p] Finished waiting for child process %d\n", wait_result);

    // Exit
    printf("[p](pid: %d) finished execution\n", getpid());
    return 0;
}