#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>

int main()
{
    // Fifo created from terminal using "mkfifo sum"

    // Create a random array
    int arr[5];
    srand(time(NULL));
    for (int i = 0; i < 5; i++)
    {
        arr[i] = rand() % 100;
        printf("Generated %d\n", arr[i]);
    }

    // Open the fifo
    printf("Opening fifo for writing\n");
    int fd = open("sum", O_WRONLY);
    if (fd == -1)
    {
        printf("Error opening fifo\n");
        return 1;
    }

    // Write the array to the fifo
    if (write(fd, arr, sizeof(arr)) == -1)
    {
        printf("Error writing to fifo\n");
        return 2;
    }
    printf("Sent the array\n");

    // Read the computed sum from the fifo
    // Open the fifo for reading
    fd = open("sum", O_RDONLY);
    if (fd == -1)
    {
        printf("Error opening fifo\n");
        return 3;
    }

    // Read the sum
    int sum;
    if (read(fd, &sum, sizeof(sum)) == -1)
    {
        printf("Error reading the sum\n");
        return 4;
    }
    printf("Received sum %d\n", sum);

    // Close fd
    close(fd);

    return 0;
}