#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

// Objective:
// 1. Child process should generate random numbers and send them to the parent
// 2. Parent should sum the numbers and print the sum

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

        // Take input from the user of number of random numbers
        int n;
        printf("[c] Enter the number of random numbers to generate: ");
        scanf("%d", &n);
        printf("[c] Generating %d random numbers\n", n);

        // Generate n random numbers
        int arr[n];
        srand(time(NULL));
        for (int i = 0; i < n; i++)
        {
            arr[i] = rand() % 10 + 1;
            printf("[c] Generated %d\n", arr[i]);
        }

        // Write n to the pipe
        if (write(fd[1], &n, sizeof(int)) == -1)
        {
            printf("Write failed\n");
            return 3;
        }

        // Write the n random numbers to the pipe
        if (write(fd[1], arr, sizeof(arr)) == -1)
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

    // Read n
    int n;
    if (read(fd[0], &n, sizeof(int)) == -1)
    {
        printf("Read failed\n");
        return 4;
    }

    // Read the array of ints
    int arr[n];
    if (read(fd[0], arr, sizeof(arr)) == -1)
    {
        printf("Read failed\n");
        return 4;
    }

    // Close the read end of the pipe
    close(fd[0]);

    // Find the sum of the numbers and print them
    int sum = 0;
    printf("[p] ");
    for (int i = 0; i < n; i++)
    {
        sum += arr[i];
        printf("%d", arr[i]);
        if (i != n - 1)
            printf(" + ");
    }
    printf(" = %d\n", sum);

    // Wait for the child and exit
    wait(NULL);
    return 0;
}