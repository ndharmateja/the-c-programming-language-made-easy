#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Objective:
// Create 10 threads, each taking a unique prime from the primes array
// and then printing it on the screen
int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void *print_prime(void *arg)
{
    // Get the index i and free the memory
    int i = *(int *)arg;
    free(arg);

    // Print the prime
    printf("Prime %d: %d\n", i, primes[i]);
    return NULL;
}

int main()
{
    // Create threads
    pthread_t th[10];
    for (int i = 0; i < 10; i++)
    {
        // 1. If we pass the address of i (&i) to the thread
        // the problem is that the value of i keeps changing at the same address
        // so we can't rely which value a particular thread would be using as it
        // only tries to get the int from the memory address where 'i' is
        // 2. So the solution is to dynamically allocate memory for each index
        // that we want to pass to the thread (this solution is generic)
        // 3. One other solution would be to pass the address of the array element
        // containing the prime number itself (primes + i) and the thread
        // can then get the prime number by doing *arg and print it
        int *i_ptr = malloc(sizeof(int));
        *i_ptr = i;
        if (pthread_create(th + i, NULL, &print_prime, (void *)i_ptr) != 0)
            return 1;

        // We cannot free that memory at this point as the thread is
        // not guaranteed to be finished yet here
        // So we free it inside the print_prime itself
    }

    // Join the threads and print the result
    for (int i = 0; i < 10; i++)
        if (pthread_join(th[i], NULL) != 0)
            return 2;

    // Exit
    return 0;
}