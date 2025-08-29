#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        printf("Pipe failed.\n");
        return 1;
    }

    int fork_id = fork();
    if (fork_id == -1)
    {
        printf("Fork failed.\n");
        return 2;
    }

    // Child process
    if (fork_id == 0)
    {
        // Close the read end of the pipe
        close(fd[0]);

        // Write to the write end of the pipe
        if (write(fd[1], "Hello world!", 12) == -1)
        {
            printf("Write failed.\n");
            return 3;
        }
        printf("Sent: 'Hello world!'\n");

        // Close the write end of the pipe
        close(fd[1]);
    }

    // Parent process
    else
    {
        // Close the write end of the pipe
        close(fd[1]);

        // Read from the read end of the pipe
        char buffer[12];
        if (read(fd[0], buffer, 12) == -1)
        {
            printf("Read failed.\n");
            return 4;
        }
        printf("Received: '%s'\n", buffer);

        // Close the read end of the pipe
        close(fd[0]);

        // Wait for the child process to finish
        wait(NULL);
    }

    return 0;
}