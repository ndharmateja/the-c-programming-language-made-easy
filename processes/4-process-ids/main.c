#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int fork_result = fork();
    if (fork_result == 0)
    {
        // Since we are waiting for the parent process to finish
        // before executing the second print statement in the child process
        // a new parent process will be assigned as the parent of this
        // child process
        printf("[child] Current ID: %d, Parent ID: %d\n", getpid(), getppid());
        sleep(2);
        printf("[child] Current ID: %d, Parent ID: %d\n", getpid(), getppid());
    }
    else
    {
        sleep(1);
        printf("[parent] Current ID: %d, Parent ID: %d\n", getpid(), getppid());
    }

    // But if we add wait(NULL) here
    // the parent process will wait for the child process to finish
    // and the second print statement in the child
    // will print the proper parent process ID
    // int wait_result = wait(NULL);
    // if (wait_result == -1)
    // {
    //     printf("No child process to wait for\n");
    // }
    // else
    // {
    //     printf("Process %d finished\n", wait_result);
    // }

    return 0;
}