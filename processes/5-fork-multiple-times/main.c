#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main()
{
    // Print the current process ID
    printf("[%d] Start! Parent ID: %d\n", getpid(), getppid());

    // Fork multiple times
    int fork_result1 = fork();
    int fork_result2 = fork();

    // The original parent process
    if (fork_result1 != 0 && fork_result2 != 0)
    {
        printf("[%d] Parent ID: %d\n", getpid(), getppid());
    }

    // The first child process (spawned from the parent process
    // in the first fork) - let us call it child1
    if (fork_result1 == 0 && fork_result2 != 0)
    {
        printf("[%d] Parent ID: %d\n", getpid(), getppid());
    }

    // The second child process (spawned from child1 process
    // in the second fork in child1) - let us call it child2
    if (fork_result1 == 0 && fork_result2 == 0)
    {
        printf("[%d] Parent ID: %d\n", getpid(), getppid());
    }

    // The second child process (spawned from the parent process
    // in the second fork) - let us call it child3
    if (fork_result1 != 0 && fork_result2 == 0)
    {
        printf("[%d] Parent ID: %d\n", getpid(), getppid());
    }

    // Wait for all the children to finish in every process
    // If wait(NULL) returns -1, it means that no children
    // to wait for. Since we are using wait(NULL) in a loop
    // it waits for all the child processes to finish.
    while (wait(NULL) != -1 || errno != ECHILD)
        ;

    return 0;
}