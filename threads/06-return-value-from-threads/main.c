#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int mails = 0;
pthread_mutex_t mutex;

void *roll_dice()
{
    // Create a dynamically allocated int on the heap
    // We can't create it on the stack as it will be lost
    // as soon as the function ends
    int *value_ptr = malloc(sizeof(int));
    *value_ptr = (rand() % 6) + 1;

    // we cast it to a void pointer
    // because the return type of the function is void * as demanded
    // by the pthread_create function
    printf("Thread result pointer: %p\n", value_ptr);
    return (void *)value_ptr;
}

int main()
{
    // Random seed
    srand(time(NULL));

    // Create threads
    pthread_t th;
    if (pthread_create(&th, NULL, &roll_dice, NULL) != 0)
        return 1;

    // Join the threads and print the result
    int *result;
    if (pthread_join(th, (void **)&result) != 0)
        return 2;
    printf("Main result pointer: %p\n", result);
    printf("You rolled a %d\n", *result);

    // Free the dynamically allocated int
    free(result);
    return 0;
}