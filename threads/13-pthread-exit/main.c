#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

// Another way to return values from threads
// pthread_exit without using a return value

void *roll_dice()
{
    // Create a dynamically allocated int on the heap
    // and store a random integer in [1, 6] in it
    int value = (rand() % 6) + 1;
    int *result_ptr = malloc(sizeof(int));
    *result_ptr = value;

    // Sleep 2 seconds
    sleep(2);

    // Using pthread_exit here instead of return
    printf("Thread result: %d\n", value);
    pthread_exit((void *)result_ptr);
}

int main()
{
    // Random seed
    srand(time(NULL));

    // Create thread
    pthread_t th[3];
    for (int i = 0; i < 3; i++)
        pthread_create(th + i, NULL, &roll_dice, NULL);

    // If we had a return 0 or exit(0) here,
    // the main function exits, and that terminates
    // all the threads it created
    // And nothing would be printed
    // exit(0);

    // But if instead we used a pthread_exit(0) here instead
    // the next lines of code after this line won't be run
    // but the thread will finish running as the process waits
    // for the threads to finish
    // So the print statement in the thread fn would run
    // Problem is freeing the dynamically allocated memory
    pthread_exit(0);

    // None of the following lines would run if we were to use pthread_exit(0)
    // Join the thread and get the result
    int *result[3];
    for (int i = 0; i < 3; i++)
        pthread_join(th[i], (void **)(result + i));
    printf("You rolled %d, %d, %d\n", *result[0], *result[1], *result[2]);

    // Free the dynamically allocated memory
    for (int i = 0; i < 3; i++)
        free(result[i]);
    return 0;
}