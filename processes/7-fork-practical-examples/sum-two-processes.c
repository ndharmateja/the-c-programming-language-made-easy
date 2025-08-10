#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 10000

int main()
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        printf("Pipe failed.\n");
        return 1;
    }

    int arr[N];
    for (int i = 0; i < N; i++)
        arr[i] = i + 1;
    int size = sizeof(arr) / sizeof(arr[0]);

    // Fork 1st process
    int fork_id1 = fork();
    if (fork_id1 == -1)
    {
        printf("Fork failed.\n");
        return 2;
    }

    // In the first child, find the sum of the first half of the array
    if (fork_id1 == 0)
    {
        // Close the read end of the pipe
        close(fd[0]);

        // Find the sum of the first half of the array
        int sum = 0;
        for (int i = 0; i < size / 2; i++)
        {
            sum += arr[i];
        }

        // Print the sum
        printf("[child] Sum of the first half of the array: %d\n", sum);

        // Write the sum to the write end of the pipe
        if (write(fd[1], &sum, sizeof(int)) == -1)
        {
            printf("Write failed.\n");
            return 3;
        }
        close(fd[1]);
    }

    // In the parent process, find the sum of the second half of the array
    else
    {
        // Close the write end of the pipe
        close(fd[1]);

        // Find the sum of the second half of the array
        int sum = 0;
        for (int i = size / 2; i < size; i++)
        {
            sum += arr[i];
        }

        // Read the sum from the read end of the pipe
        int sum2;
        if (read(fd[0], &sum2, sizeof(int)) == -1)
        {
            printf("Read failed.\n");
            return 4;
        }
        close(fd[0]);

        // Print the sum
        printf("[parent] Sum of the second half of the array: %d\n", sum);
        printf("[parent] Sum of the total array: %d\n", sum + sum2);

        // Wait for the first child to finish
        wait(NULL);
    }

    return 0;
}