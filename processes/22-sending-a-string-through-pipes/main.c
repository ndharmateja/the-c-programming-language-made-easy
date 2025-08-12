#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Objective:
// Send a user inputted string from the child process
// to the parent process
int main()
{
    // Create pipe
    int fd[2];
    if (pipe(fd) == -1)
    {
        printf("Pipe failed.\n");
        return 1;
    }

    // Fork a child process
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

        // Read the string
        char line[200];
        printf("[c] Enter a string: ");
        fgets(line, 200, stdin);

        // Remove the \n at the end
        line[strlen(line) - 1] = '\0';
        printf("[c] User entered '%s'\n", line);

        // Write the string + \0 to the pipe, that is why the len + 1
        if (write(fd[1], line, strlen(line) + 1) == -1)
        {
            printf("Write failed\n");
            return 3;
        }

        // Close the write end of the pipe
        close(fd[1]);

        // Exit
        return 0;
    }

    // Parent process
    // Close the write end of the pipe
    close(fd[1]);

    // Read the string byte by byte until the null terminator is reached
    // (including the null terminator)
    // This means multiple read operations
    // A more efficient way might be to send the length of the string ahead
    // of the string itself
    char line[200];
    int i = 0;
    char c;
    while (1)
    {
        if (read(fd[0], &c, sizeof(char)) == -1)
        {
            printf("Read failed\n");
            return 4;
        }

        // If we read a null terminator
        // we can stop reading
        if (c == '\0')
            break;

        // Increment i and assign c
        line[i++] = c;
    }
    printf("[p] Read %d bytes\n", i);

    // Print the received string
    printf("[p] Received '%s' from the child\n", line);

    // Wait for the child and exit
    wait(NULL);
    return 0;
}