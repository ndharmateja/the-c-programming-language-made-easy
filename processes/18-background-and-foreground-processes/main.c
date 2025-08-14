#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

// We can use "ctrl+z" to send the process to the background
// Processes that are stopped (paused) are sent to the background
// In the terminal, we can use "fg" to bring the process back to the foreground
int main()
{
    while (1)
    {
        printf("Hello\n");
        sleep(1);
    }
    return 0;
}