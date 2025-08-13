#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// Objective:
// Create an int in the main process and send it to one child
// process and increment it by 5 and then send that result to
// the other child process and increment it by 5 again
// and send that result back to the main process
// We need three pipes for this

int main()
{
    // Create three pipes
    // Warning: Handle errors properly
    // if the creation of the third pipe fails, we should close the first two
    // opened pipes etc
    int fd_p_c1[2], fd_c1_c2[2], fd_c2_p[2];
    if (pipe(fd_p_c1) == -1 || pipe(fd_c1_c2) == -1 || pipe(fd_c2_p) == -1)
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

    // Child process 1
    if (fork_id1 == 0)
    {
        // Close read and write ends of the pipe between child 2 and parent
        close(fd_c2_p[0]);
        close(fd_c2_p[1]);

        // Close the write end of the pipe between parent and child 1
        close(fd_p_c1[1]);

        // Close the read end of the pipe between child 1 and child 2
        close(fd_c1_c2[0]);

        // Read an integer sent by the parent
        // Close the read end of the pipe between parent and child 1
        int x;
        if (read(fd_p_c1[0], &x, sizeof(int)) == -1)
        {
            printf("Read failed\n");
            return 3;
        }
        printf("[c1] Received %d from parent\n", x);
        close(fd_p_c1[0]);

        // Increment it by 5 and send it to child 2
        // Close the write end of the pipe between child 1 and child 2
        x += 5;
        printf("[c1] Incremented the value by 5: %d\n", x);
        if (write(fd_c1_c2[1], &x, sizeof(int)) == -1)
        {
            printf("Write failed\n");
            return 4;
        }
        printf("[c1] Sent %d to child 2\n", x);
        close(fd_c1_c2[1]);

        // Exit
        return 0;
    }

    // Second child process
    int fork_id2 = fork();
    if (fork_id2 == -1)
    {
        printf("Fork failed.\n");
        return 5;
    }

    // Child process 2
    if (fork_id2 == 0)
    {
        // Close read and write ends of the pipe between parent and child 1
        close(fd_p_c1[0]);
        close(fd_p_c1[1]);

        // Close the write end of the pipe between child 1 and child 2
        close(fd_c1_c2[1]);

        // Close the read end of the pipe between child 2 and parent
        close(fd_c2_p[0]);

        // Read an integer sent by child 1
        // Close the read end of the pipe between child 1 and child 2
        int x;
        if (read(fd_c1_c2[0], &x, sizeof(int)) == -1)
        {
            printf("Read failed\n");
            return 6;
        }
        printf("[c2] Received %d from child 1\n", x);
        close(fd_c1_c2[0]);

        // Increment it by 5 and send it to child 2
        // Close the write end of the pipe between child 2 and parent
        x += 5;
        printf("[c2] Incremented the value by 5: %d\n", x);
        if (write(fd_c2_p[1], &x, sizeof(int)) == -1)
        {
            printf("Write failed\n");
            return 7;
        }
        printf("[c2] Sent %d to parent\n", x);
        close(fd_c2_p[1]);

        // Exit
        return 0;
    }

    // Parent process
    // Close read and write ends of the pipe between child 1 and child 2
    close(fd_c1_c2[0]);
    close(fd_c1_c2[1]);

    // Close the write end of the pipe between child 2 and parent
    close(fd_c2_p[1]);

    // Close the read end of the pipe between parent and child 1
    close(fd_p_c1[0]);

    // Send an integer to child 1
    // Close the write end of the pipe between parent and child 1
    int x = 42;
    if (write(fd_p_c1[1], &x, sizeof(int)) == -1)
    {
        printf("Write failed\n");
        return 8;
    }
    printf("[p] Sent %d to child 1\n", x);
    close(fd_p_c1[1]);

    // Read an integer sent by child 2
    // Close the read end of the pipe between child 2 and parent
    if (read(fd_c2_p[0], &x, sizeof(int)) == -1)
    {
        printf("Read failed\n");
        return 9;
    }
    printf("[p] Received %d from child 2\n", x);
    close(fd_c2_p[0]);

    // Wait for both child processes to finish
    waitpid(fork_id1, NULL, 0);
    waitpid(fork_id2, NULL, 0);

    return 0;
}