#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

// Handling sigtstp signal
// It will override the stopping or pausing of the process
// and simply execute this function handler
// Upon getting the SIGTSTP signal, it won't stop the process
// but simply execute this handler
void handle_sigtstp(int sig)
{
    printf("Received SIGTSTP (%d)\n", sig);
}

// Handling sigcnt signal
// Along with the function handler being executed on SIGCONT signal,
// the process will also be continued
// SIGCONT is special, so even though we can handle the signal
// and execute the function handler, it will also continue the process
void handle_sigcnt(int sig)
{
    printf("Received SIGCNT (%d)\n", sig);
}

int main()
{
    // Create a signal handler and bind it to SIGCONT signal
    struct sigaction sa;
    sa.sa_handler = &handle_sigcnt;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCONT, &sa, NULL);

    while (1)
    {
        printf("Hello\n");
        sleep(1);
    }

    return 0;
}