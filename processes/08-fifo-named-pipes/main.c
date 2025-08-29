#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

int main()
{
    // Create the named pipe
    if (mkfifo("myfifo1", 0777) == -1)
    {
        // If the named pipe doesn't exist
        // and an error is caused (-1)
        if (errno != EEXIST)
        {
            printf("Error creating fifo\n");
            return 1;
        }

        // Otherwise it means that the named pipe already exists
        // So we don't have to do anything
        printf("Named pipe already exists\n");
    }
    else
    {
        // At this point the named pipe "myfifo1" exists
        printf("Named pipe created\n");
    }

    // Open the named pipe
    // From man page: FIFOs
    //    Opening the read or write end of a FIFO blocks until the other end
    //    is also OPENED (by another process or thread) for write or read (respectively).
    // We could simulate that behavior by using "cat myfifo1" from
    // another terminal process and then the program will continue running
    printf("Opening named pipe\n");
    int fd = open("myfifo1", O_WRONLY);
    if (fd == -1)
    {
        printf("Error opening fifo\n");
        return 2;
    }

    printf("Opened named pipe\n");

    // Write to the named pipe and close the file descriptor
    // ASCII code for 'a'
    int x = 97;
    if (write(fd, &x, sizeof(x)) == -1)
    {
        printf("Error writing to fifo\n");
        return 3;
    }
    printf("Wrote to named pipe\n");

    // Close the file descriptor
    close(fd);

    return 0;
}