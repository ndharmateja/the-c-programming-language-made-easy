#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

// Objective:
// Ask the user a question. If the user doesn't answer in 5 sec,
// we need to provide the user with a hint.
// The idea is to use signals to communicate between processes
// We wait for 5 seconds in the child process and then send a signal
// to the parent process where we can use a signal handler to show the hint
// We also kill the child process as soon as the user enters the answer
// so that it doesn't keep running in the background (sleep and hint)

void handle_sigusr1(int sig)
{
    printf("\nHint: Multiplication is just repeated addition.\n");
    printf("What is 3x5? ");
    fflush(stdout);
}

int main()
{
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
        // Sleep 5 seconds and then send a signal (user signal type) to the parent
        sleep(5);
        kill(getppid(), SIGUSR1);

        // Exit so that we don't need the else for the parent process
        return 0;
    }

    // Parent process
    // Bind the SIGUSR1 signal to a function handler
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &handle_sigusr1;
    sigaction(SIGUSR1, &sa, NULL);

    // User input logic
    int result;
    printf("What is 3x5? ");
    scanf("%d", &result);

    // Once the user answers the question, we kill the child process
    // so that it doesn't sleep for 5 sec when the user answers immediately
    kill(fork_id, SIGKILL);
    if (result == 15)
        printf("Correct!\n");
    else
        printf("Incorrect!\n");

    // Exit
    return 0;
}