#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main()
{
    printf("*** Program started ***\n");

    // Fork a child process
    int fork_id = fork();
    if (fork_id == -1)
    {
        printf("Fork failed.\n");
        return 1;
    }

    // Run the ping command in the child process
    if (fork_id == 0)
    {
        if (execlp("ping", "ping", "-c", "3", "www.google.com", NULL) == -1)
        {
            printf("execlp failed\n");
            int errno_val = errno;
            printf("errno: %d\n", errno_val);
            printf("line after execl\n");
            return 2;
        }

        printf("This shouldn't be printed\n");
    }

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

    printf("*** Program finished ***\n");
}