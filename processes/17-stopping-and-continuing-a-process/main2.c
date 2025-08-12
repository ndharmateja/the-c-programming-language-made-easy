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
        // This could be some resource intensive process
        // like an event listener listening for something over the network
        // and using signals, we can pause the process and then continue it etc
        while (1)
        {
            printf("infinity\n");
            usleep(500000);
        }
    }

    // Pause the child process
    int t;
    kill(fork_id, SIGSTOP);

    do
    {
        printf("Enter number of seconds of execution: ");
        scanf("%d", &t);

        // Continue the child process for t seconds
        // then pause the child process
        if (t > 0)
        {
            kill(fork_id, SIGCONT);
            sleep(t);
            kill(fork_id, SIGSTOP);
        }
    } while (t > 0);

    // Parent process
    kill(fork_id, SIGKILL);

    return 0;
}