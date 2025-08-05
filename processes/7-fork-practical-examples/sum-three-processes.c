#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define N 10000

int main()
{
    int arr[N];
    for (int i = 0; i < N; i++)
        arr[i] = i + 1;
    int size = sizeof(arr) / sizeof(arr[0]);

    // Create pipe
    // fd[0] - read, fd[1] - write
    int fd[2];
    if (pipe(fd) == -1)
    {
        printf("Pipe failed.\n");
        return 1;
    }

    // Fork 1st process
    int fork_id1 = fork();
    if (fork_id1 == -1)
    {
        printf("Fork failed.\n");
        return 2;
    }

    // In the first child, find the sum of the first third elements
    if (fork_id1 == 0)
    {
        // Close the read end of the pipe
        close(fd[0]);

        // find the sum of elements
        int sum = 0, start = 0, end = size / 3;
        for (int i = start; i < end; i++)
            sum += arr[i];
        printf("Calculated partial sum: %d\n", sum);

        // Write sum to the pipe and close the write end
        if (write(fd[1], &sum, sizeof(sum)) == -1)
        {
            printf("Write from the second child failed.\n");
            return 3;
        }
        close(fd[1]);

        // Exit
        return 0;
    }

    // Fork 2nd process
    int fork_id2 = fork();
    if (fork_id2 == -1)
    {
        printf("Fork failed.\n");
        return 2;
    }

    // In the second child, find the sum of the middle third elements
    if (fork_id2 == 0)
    {
        // Close the read end of the pipe
        close(fd[0]);

        // find the sum of elements
        int sum = 0, start = size / 3, end = 2 * size / 3;
        for (int i = start; i < end; i++)
            sum += arr[i];
        printf("Calculated partial sum: %d\n", sum);

        // Write sum to the pipe and close the write end
        if (write(fd[1], &sum, sizeof(sum)) == -1)
        {
            printf("Write from the second child failed.\n");
            return 3;
        }
        close(fd[1]);

        // Exit
        return 0;
    }

    // In the parent, close the write end of the pipe
    close(fd[1]);

    // Find the sum of the last third elements
    // find the sum of elements
    int sum = 0, start = 2 * size / 3, end = size;
    for (int i = start; i < end; i++)
        sum += arr[i];
    printf("Calculated partial sum: %d\n", sum);

    // Read the sums computed by the children processes
    int sum1, sum2;
    if (read(fd[0], &sum1, sizeof(sum1)) == -1 || read(fd[0], &sum2, sizeof(sum2)) == -1)
    {
        printf("Read in parent failed.\n");
        return 4;
    }

    // Wait for all children to be done
    while (wait(NULL) != -1 || errno != ECHILD)
        ;

    // Print the total sum
    sum += sum1 + sum2;
    printf("Total sum: %d\n", sum);
    return 0;
}