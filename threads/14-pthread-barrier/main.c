#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 6

// Thread barriers are used to synchronize threads
// that are running in parallel
// They stop the threads from executing until there are
// a certain number of threads that have reached the barrier
// and it allows all those threads to continue executing
//
// Eg:
// Say the barrier is defined for 3 threads
// If 2 threads have reached the barrier, they will have to wait
// until the 3rd thread has reached the barrier

pthread_barrier_t barrier;

void *f()
{
    printf("Waiting at the barrier\n");

    // Here, this barrier will wait until 3 threads have reached it
    // If NUM_THREADS were 2, this barrier would never be broken
    // and they will wait forever
    pthread_barrier_wait(&barrier);
    printf("Reached the barrier\n");

    return NULL;
}

int main()
{
    // Initialize the barrier with 3 threads
    pthread_barrier_init(&barrier, NULL, 3);

    // Create NUM_THREADS threads
    pthread_t th[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(th + i, NULL, &f, NULL);
        
        // We can very clearly see the behaviour of the barrier
        // by adding a sleep here
        // So everytime three threads are created, they all
        // pass through the barrier at the same time
        sleep(1);
    }

    // Join the threads
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(th[i], NULL);

    // Destroy the barrier
    pthread_barrier_destroy(&barrier);
    return 0;
}
