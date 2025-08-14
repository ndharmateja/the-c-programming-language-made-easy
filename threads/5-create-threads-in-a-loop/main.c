#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TEN_MILLION 10000000
#define NUM_THREADS 8

int mails = 0;
pthread_mutex_t mutex;

void *f()
{
    for (int i = 0; i < TEN_MILLION; i++)
    {
        // Lock the mutex while one thread is doing the mails++
        // so that the other thread has to wait until the mutex is unlocked
        // That way there wouldn't be any race conditions
        // So we would get the correct result: 20_000_000
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // Create threads in a loop
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_create(threads + i, NULL, &f, NULL) != 0)
            return 1;
        printf("Thread %d started\n", i);
    }

    // Join the threads
    // We should not have the pthread_join inside the above for loop
    // as then the threads wouldn't run in parallel
    // The first thread would be created and run and then the join waits
    // until the first thread finishes and the same thing happens for the
    // remaining threads
    // And the print statements of finishing would be in order 0, 1, 2..
    // even though the order of threads finishing is not necessarily the same
    // This happens because we are waiting for each thread to join (or finish)
    // in a loop sequentially and then printing
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
            return 2;
        printf("Thread %d finished\n", i);
    }

    // Destroy mutex
    pthread_mutex_destroy(&mutex);

    // If number of threads is 8
    // mails should be equal to 80_000_000
    printf("Mails: %d\n", mails);
    return 0;
}