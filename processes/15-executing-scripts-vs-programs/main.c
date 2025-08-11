#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main()
{
    int fork_id = fork();
    if (fork_id == -1)
    {
        printf("Fork failed.\n");
        return 1;
    }

    // Child process
    if (fork_id == 0)
    {
        // Exec only executes executable
        // so pipe operators etc won't work
        // as exec is not a shell

        // This would not work the same as the command pipe
        // ping -c 3 | grep "round-trip"
        if (execlp("ping", "ping", "-c", "3", "|", "grep", "\"round-trip\"", NULL) == -1)
        {
            printf("execlp failed\n");
            int errno_val = errno;
            printf("errno: %d\n", errno_val);
            return 2;
        }
    }

    // Parent process
    // Wait for the child process to finish
    int wait_status;
    wait(&wait_status);
    if (WIFEXITED(wait_status))
    {
        // If child process is successful (status code == 0)
        int status_code = WEXITSTATUS(wait_status);
        if (status_code == 0)
            printf("Ping finished successfully\n");

        // If child process fails (status code > 0)
        else
            printf("Ping failed with status code: %d\n", status_code);
    }

    return 0;
}