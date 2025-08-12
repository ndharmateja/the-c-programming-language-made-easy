#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int main()
{
    // With pipes we can send bytes of data
    // but with signals we just send an integer signal
    // to a process to communicate with it
    // say to kill/pause etc a process

    // Fork a child process
    int fork_id = fork();
    if (fork_id == -1)
    {
        printf("Fork failed.\n");
        return 1;
    }

    // Child process
    if (fork_id == 0)
    {
        // Print "infinity" every 0.5 seconds
        while (1)
        {
            printf("infinity\n");
            usleep(500000);
        }
    }

    // Parent process
    // Wait for 3 sec and then pause the child process
    printf("***Waiting for 3 seconds before pausing the child process***\n");
    sleep(3);
    kill(fork_id, SIGSTOP);
    printf("***Paused the child process for 4 sec***\n");

    // Wait for 4 sec and then continue the child process
    printf("***Waiting for 4 seconds before continuing the child process***\n");
    sleep(4);
    kill(fork_id, SIGCONT);
    printf("***Continued the child process***\n");

    // Child's process id is stored in fork_id
    // Wait for 2 seconds and then kill the child process
    // using SIGKILL (the function is 'kill' no matter what
    // signal we want to send)
    printf("***Waiting for 2 seconds before killing the child process***\n");
    sleep(2);
    kill(fork_id, SIGKILL);
    printf("***Killed the child process***\n");

    printf("\n***End of the parent process***\n");
    return 0;
}