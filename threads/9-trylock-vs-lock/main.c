#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t mutex;

void *f(void *arg)
{
    // When a thread is locked, other threads can't get the lock
    // so they pause.
    // Now it just becomes a sequential execution as each thread's function
    // is locked at the beginning and unlocked at the end.
    // The pthread_mutex_lock function waits until another thread releases the lock
    // before it can get the lock.
    //
    // The expected output for 4 threads would be:
    // Got lock!
    // <sleep 1>
    // Released lock!
    // Got lock!
    // <sleep 1>
    // Released lock!
    // Got lock!
    // <sleep 1>
    // Released lock!
    // Got lock!
    // <sleep 1>
    // Released lock!
    pthread_mutex_lock(&mutex);
    printf("Got lock!\n");
    sleep(1);
    pthread_mutex_unlock(&mutex);
    printf("Released lock!\n");
    return NULL;
}

void *g(void *arg)
{
    // When we use trylock the thread tries to get the lock
    // If it succeeds, the pthread_mutex_trylock returns 0
    // Otherwise it wouldn't try to get the lock and continues running the next code
    //
    // So the expected output for 4 threads would be (as the other 3 threads
    // fail to get the lock as the first thread got the lock and is sleeping):
    // Got lock!
    // Couldn't get lock!
    // Couldn't get lock!
    // Couldn't get lock!
    // <sleep 1>
    // Released lock!
    if (pthread_mutex_trylock(&mutex) == 0)
    {
        printf("Got lock!\n");
        sleep(1);
        pthread_mutex_unlock(&mutex);
        printf("Released lock!\n");
        return NULL;
    }
    printf("Couldn't get lock!\n");
    return NULL;
}

int main()
{
    // Create the thread args and the threads
    // Add error handling
    pthread_t th[4];

    // Run with f
    printf("Running with f (pthread_mutex_lock):\n");
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 4; i++)
        pthread_create(th + i, NULL, &f, NULL);

    // Join the threads and print the result
    for (int i = 0; i < 4; i++)
        pthread_join(th[i], NULL);

    // Run with g
    printf("\nRunning with g (pthread_mutex_trylock):\n");
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 4; i++)
        pthread_create(th + i, NULL, &g, NULL);

    // Join the threads and print the result
    for (int i = 0; i < 4; i++)
        pthread_join(th[i], NULL);

    // Exit
    return 0;
}