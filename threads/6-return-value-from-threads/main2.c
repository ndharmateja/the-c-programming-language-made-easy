#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define TEN_MILLION 10000000
#define NUM_THREADS 8

// Objective:
// Get a dice roll from each of the threads and then print all of them
// in the main fun

int mails = 0;
pthread_mutex_t mutex;

void *roll_dice()
{
    // Create a dynamically allocated int on the heap
    // We can't create it on the stack as it will be lost
    // as soon as the function ends
    int *value_ptr = malloc(sizeof(int));
    *value_ptr = (rand() % 6) + 1;
    printf("Generated %d\n", *value_ptr);

    // we cast it to a void pointer
    // because the return type of the function is void * as demanded
    // by the pthread_create function
    return (void *)value_ptr;
}

int main()
{
    // Random seed
    srand(time(NULL));

    // To store the results of the threads
    int *result[NUM_THREADS];

    // Create threads
    pthread_t th[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
        if (pthread_create(&th[i], NULL, &roll_dice, NULL) != 0)
            return 1;

    // Join the threads and print the result
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_join(th[i], (void **)(result + i)) != 0)
            return 2;
        printf("[thread %d] You rolled a %d\n", i, *result[i]);

        // Free the dynamically allocated memory for each of the threads
        free(result[i]);
    }

    // Exit
    return 0;
}