#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

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
        // We redirect the ping output to a file
        // basically the stdout of this child process is redirected
        // 'file' is probably 3
        int file = open("ping_output.txt", O_WRONLY | O_CREAT, 0777);
        if (file == -1)
        {
            printf("Failed to open file\n");
            return 2;
        }
        printf("The fd to 'ping_output.txt' is %d\n", file);

        // File descriptors (opened by default by the OS)
        // 0 - stdin
        // 1 - stdout
        // 2 - stderr
        // 3 - ping_output.txt

        // Redirect such that 1 means ping_output.txt
        // 'file2' is 1
        // This is going to close stdout completely
        // and it is going to duplicate the file "ping_output.txt"
        // Now both 1 and 3 point to the same file "ping_output.txt"
        // Now any output to the stdout is going to go to "ping_output.txt"
        int file2 = dup2(file, STDOUT_FILENO);
        if (file2 == -1)
        {
            printf("Failed to redirect stdout\n");
            return 2;
        }
        printf("The duplicate fd to 'ping_output.txt' is %d\n", file2);

        // We can close the fd 'file' as it is not needed anymore
        // and 1 is going to point to "ping_output.txt"
        close(file);

        // Execute the ping command
        if (execlp("ping", "ping", "-c", "3", "www.google.com", NULL) == -1)
        {
            printf("execlp failed\n");
            int errno_val = errno;
            printf("errno: %d\n", errno_val);
            printf("line after execl\n");
            return 3;
        }

        printf("This shouldn't be printed\n");
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

    printf("*** Program finished ***\n");
}