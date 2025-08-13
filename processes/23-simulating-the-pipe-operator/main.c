#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// This would not work the same
// execlp("ping", "ping", "-c", "5", "|", "grep", "ms", NULL)
// as the command pipe
// ping -c 5 | grep ms

// To simulate a pipe, we have to create the pipe and
// we have to use the pipe as the stdout for the ping command
// and use the same pipe as the stdin for the grep command

int main()
{
    // Create a pipe
    int fd[2];
    if (pipe(fd) == -1)
    {
        printf("Pipe failed.\n");
        return 1;
    }

    int fork_id1 = fork();
    if (fork_id1 == -1)
    {
        printf("Fork failed.\n");
        return 2;
    }

    // Child process 1 for ping
    if (fork_id1 == 0)
    {
        // Close the read end of the pipe
        close(fd[0]);

        // Duplicate the write end of the pipe and make 1 (stdout file number)
        // point to it, and close the original write end
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        // Run the exec command with ping
        if (execlp("ping", "ping", "-c", "5", "google.com", NULL) == -1)
        {
            printf("execlp failed\n");
            int errno_val = errno;
            printf("errno: %d\n", errno_val);
            return 3;
        }
    }

    // Second child process
    int fork_id2 = fork();
    if (fork_id2 == -1)
    {
        printf("Fork failed.\n");
        return 4;
    }

    // Child process 2 for grep
    if (fork_id2 == 0)
    {
        // Close the write end of the pipe
        close(fd[1]);

        // Duplicate the read end of the pipe and make 0 (stdin file number)
        // point to it, and close the original read end
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        // Run the exec command with grep
        if (execlp("grep", "grep", "ms", NULL) == -1)
        {
            printf("execlp failed\n");
            int errno_val = errno;
            printf("errno: %d\n", errno_val);
            return 5;
        }
    }

    // Close both ends of the pipe in the parent process
    // and closing the write end also tells the grep command that
    // there is no more input and it stops looking for more inputs
    close(fd[0]);
    close(fd[1]);

    // Wait for both child processes to finish
    waitpid(fork_id1, NULL, 0);
    waitpid(fork_id2, NULL, 0);

    return 0;
}