#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Detached threads are generally used when we have long running processes
// where we don't want to wait for the threads to finish
// Two ways to create detached threads:
// 1. Use pthread_attr_t while creating the thread itself (We can prefer
// this as there is a possibility of the thread finishing between the calls
// to pthread_create and pthread_detach => resources won't be freed as it won't
// be joined and it wasn't in a detached state)
//    - pthread_t th;
//    - pthread_attr_t detached;
//    - pthread_attr_init(&detached);
//    - pthread_attr_setdetachstate(&detached, PTHREAD_CREATE_DETACHED);
//    - pthread_create(&th, &detached, f, NULL);
//    - ... <some code>
//    - pthread_attr_destroy(&detached);
// 2. Use pthread_detach after creating the thread (We can use this to dynamically
// detach the threads some time after the thread is created)
//    - pthread_t th;
//    - pthread_create(&th, NULL, f, NULL);
//    - ... <some code>
//    - pthread_detach(th);

#define NUM_THREADS 2

void *f(void *arg)
{
    printf("Hello from thread\n");
    sleep(1);
    printf("Goodbye from thread\n");

    return NULL;
}

int main()
{
    // Create threads with error handling
    pthread_t th[NUM_THREADS];

    // Create the detached attribute
    pthread_attr_t detached;
    pthread_attr_init(&detached);
    pthread_attr_setdetachstate(&detached, PTHREAD_CREATE_DETACHED);

    // Create the detached threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_create(th + i, &detached, f, NULL) != 0)
        {
            perror("Failed to create thread");
            return 1;
        }

        // Threads can be detached here as well
        // but there is a possibility of the thread finishing
        // execution before the pthread_detach is even called
        // pthread_detach(th[i]);
    }

    // We cannot join a detached thread
    // Detached threads clear their own resources
    // We shouldn't join them
    // Joining them will cause an error
    for (int i = 0; i < NUM_THREADS; i++)
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Failed to join thread");
        }

    // Destroy the detached attribute
    pthread_attr_destroy(&detached);

    // Exit from the thread instead of return
    // so that the main thread exits but the other threads won't
    // If we had used return instead, the "Hello from thread" would print twice
    // but the "Goodbye from thread" would not print as the whole process
    // exits while the threads are in sleep
    // If we use pthread_exit, the process is going to wait for all
    // the threads to finish before exiting
    pthread_exit(0);
}