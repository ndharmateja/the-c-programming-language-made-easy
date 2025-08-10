#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    // The main issue is that what we write from the parent process
    // could be immediately read by the parent process itself as there
    // is nothing stopping it from doing it as it is the same pipe
    // So the solution is to have two pipes, one pipe for parent to child
    // communication and another for child to parent communication
    int pipe_p2c[2], pipe_c2p[2];
    if (pipe(pipe_p2c) == -1 || pipe(pipe_c2p) == -1)
    {
        printf("Error creating pipe\n");
        return 1;
    }

    int fork_result = fork();
    if (fork_result == -1)
    {
        printf("Error forking\n");
        return 2;
    }

    // Child process
    if (fork_result == 0)
    {
        // Close the read end of the c2p pipe
        // and the write end of the p2c pipe
        close(pipe_c2p[0]);
        close(pipe_p2c[1]);

        // Read an int from the p2c pipe
        int x;
        if (read(pipe_p2c[0], &x, sizeof(int)) == -1)
        {
            printf("Error reading\n");
            return 3;
        }
        printf("[child] Received int %d\n", x);

        // Perform computation
        x *= 4;

        // Write the computed value to the c2p pipe
        if (write(pipe_c2p[1], &x, sizeof(int)) == -1)
        {
            printf("Error writing\n");
            return 4;
        }
        printf("[child] Sent the computed value %d\n", x);

        // Close the remaining fds, the write end of the c2p pipe
        // and the read end of the p2c pipe
        close(pipe_c2p[1]);
        close(pipe_p2c[0]);
    }

    // Parent process
    else
    {
        // Close the read end of the p2c pipe
        // and the write end of the c2p pipe
        close(pipe_p2c[0]);
        close(pipe_c2p[1]);

        // Write an int to the p2c pipe
        int x = 5;
        if (write(pipe_p2c[1], &x, sizeof(int)) == -1)
        {
            printf("Error writing\n");
            return 5;
        }
        printf("[parent] Sent int %d\n", x);

        // Read an int from the c2p pipe
        if (read(pipe_c2p[0], &x, sizeof(int)) == -1)
        {
            printf("Error reading\n");
            return 6;
        }
        printf("[parent] Received the computed value %d\n", x);

        // Close the remaining fds, the write end of the p2c pipe
        // and the read end of the c2p pipe
        close(pipe_p2c[1]);
        close(pipe_c2p[0]);

        // Wait for child to finish
        wait(NULL);
    }

    return 0;
}