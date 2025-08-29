#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>

void print_array(int *arr, int size)
{
    printf("[ ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("]\n");
}

int main()
{
    int arr[5];

    // Open the fifo for reading
    printf("Opening fifo for reading\n");
    int fd = open("sum", O_RDONLY);
    if (fd == -1)
    {
        printf("Error opening fifo\n");
        return 1;
    }

    // Read the array from the fifo
    if (read(fd, arr, sizeof(arr)) == -1)
    {
        printf("Error reading from fifo\n");
        return 2;
    }
    printf("Received the array: ");
    print_array(arr, 5);

    // Close the fifo
    close(fd);

    // Find the sum of the array
    int sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += arr[i];
    }
    printf("Sum computed: %d\n", sum);

    // Write the computed sum to the fifo
    // Open the fifo for writing
    fd = open("sum", O_WRONLY);
    if (fd == -1)
    {
        printf("Error opening fifo\n");
        return 3;
    }

    // Write the sum
    if (write(fd, &sum, sizeof(sum)) == -1)
    {
        printf("Error writing the sum\n");
        return 4;
    }
    printf("Sent sum %d\n", sum);

    // Close fd
    close(fd);

    return 0;
}