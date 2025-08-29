#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

// Objective:
// Create a value and pass it around multiple times through n children processes
// each time incrementing its value and then pass it back to the parent
//
// eg: say 3 child processes (other than the main process)
// say initial value is 4
// say increment is 5
// 4 => 4+5=9 => 9+5=14 => 14+5=19
// We should get back 19 in the main process

int main()
{
    // If there are n children processes + one main process
    // we need n + 1 pipes
    // Main process (process 0), process 1, process 2, ..., process n
    //
    // We store the pipes as a 2d array
    // The ith pipe is used as writing from ith process to
    // reading from the i+1 th process
    // 0th pipe - main process to process 1
    // 1st pipe - process 1 to process 2
    // ...
    // n-1th pipe - process n-1 to process n
    // nth pipe - process n to main process
    //
    // Main process
    // <pipe 0>
    // Child process 1
    // <pipe 1>
    // Child process 2
    // <pipe 2>
    // ...
    // Child process n-1
    // <pipe n-1>
    // Child process n
    // <pipe n>
    // Main process
    //
    // ith process writes to pipe[i][1] and reads from pipe[i-1][0]

    // Take input of initial value, number of child processes and increment
    int x, n, inc;
    printf("[p] Enter the initial value: ");
    scanf("%d", &x);
    printf("[p] Enter the number of child processes: ");
    scanf("%d", &n);
    printf("[p] Enter the increment value: ");
    scanf("%d", &inc);

    // Create n+1 pipes
    int pipes[n + 1][2];
    for (int i = 0; i < n + 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            printf("Pipe failed.\n");
            return 1;
        }
    }

    // Fork n child processes
    // In each process, we simply return at the end
    // so we don't need any nested if statements
    for (int i = 1; i < n + 1; i++)
    {
        int fork_id = fork();
        if (fork_id == -1)
        {
            printf("Fork failed.\n");
            return 2;
        }

        // Child processes
        if (fork_id == 0)
        {
            // Close all the irrelevant pipes to pipe i
            // Only relevant pipe ends are
            // writing to pipes[i][1]
            // reading from pipes[i-1][0]
            for (int j = 0; j < n + 1; j++)
            {
                // Don't close pipes[i][1]
                if (j != i)
                    close(pipes[j][1]);

                // Don't close pipes[i-1][0]
                if (j != i - 1)
                    close(pipes[j][0]);
            }

            // Read an integer from the i-1 th process
            // Close the appropriate end
            if (read(pipes[i - 1][0], &x, sizeof(int)) == -1)
            {
                printf("Read failed.\n");
                return 3;
            }
            printf("[c%d] Received %d\n", i, x);
            close(pipes[i - 1][0]);

            // Increment it
            x += inc;

            // Write it back to the i+1 th process
            // Close the appropriate end
            if (write(pipes[i][1], &x, sizeof(int)) == -1)
            {
                printf("Write failed.\n");
                return 4;
            }
            printf("[c%d] Sent %d\n", i, x);
            close(pipes[i][1]);

            // Exit
            return 0;
        }
    }

    // Parent process
    // Close all the irrelavant pipes except
    // writing to pipes[0][1]
    // reading from pipes[n][0]
    for (int i = 0; i < n + 1; i++)
    {
        // Don't close pipes[0][1]
        if (i != 0)
            close(pipes[i][1]);

        // Don't close pipes[n][0]
        else if (i != n)
            close(pipes[i][0]);
    }

    // Write an integer to the 1st process
    // Close the appropriate end
    if (write(pipes[0][1], &x, sizeof(int)) == -1)
    {
        printf("Write failed.\n");
        return 4;
    }
    printf("[p] Sent %d\n", x);
    close(pipes[0][1]);

    // Read an integer from the nth process
    // Close the appropriate end
    if (read(pipes[n][0], &x, sizeof(int)) == -1)
    {
        printf("Read failed.\n");
        return 3;
    }
    printf("[p] Received %d\n", x);
    close(pipes[n][0]);

    // Wait for all the child processes to finish
    while (wait(NULL) != -1 || errno != ECHILD)
        ;

    // Exit
    printf("[p] finished execution\n");
    return 0;
}