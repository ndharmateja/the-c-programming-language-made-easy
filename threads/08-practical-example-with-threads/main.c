#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Objective:
// Create 2 threads, find the sum of the first half and the second half
// in each of the threads and then get the resulting sum in the main
#define NUM_ELEMENTS 10
int primes[NUM_ELEMENTS] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

// Define a struct to store the arg to the function
typedef struct
{
    int start;
    int end;
} thread_args_t;

void *sum_elements(void *arg)
{
    // Create a pointer to store the sum
    int *sum_ptr = (int *)malloc(sizeof(int));
    thread_args_t *args = (thread_args_t *)arg;

    // Find the sum of the given range
    int sum = 0;
    int start = args->start;
    int end = args->end;
    for (int i = start; i < end; i++)
        sum += primes[i];
    printf("Thread sum: %d\n", sum);

    // Assign the sum to the pointer
    *sum_ptr = sum;
    return sum_ptr;
}

int main()
{
    // Create the thread args and the threads
    // Add error handling
    pthread_t t0, t1;
    thread_args_t args0 = {0, NUM_ELEMENTS / 2};
    thread_args_t args1 = {NUM_ELEMENTS / 2, NUM_ELEMENTS};
    pthread_create(&t0, NULL, sum_elements, &args0);
    pthread_create(&t1, NULL, sum_elements, &args1);

    // Join the threads and print the result
    int *left_sum_ptr, *right_sum_ptr;
    pthread_join(t0, (void **)&left_sum_ptr);
    pthread_join(t1, (void **)&right_sum_ptr);
    printf("Total sum: %d\n", *left_sum_ptr + *right_sum_ptr);

    // Free the dynamically allocated memory
    free(left_sum_ptr);
    free(right_sum_ptr);

    // Exit
    return 0;
}