#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>

#define NUM_THREADS 4

void *f()
{
    sleep(1);

    // This should not be done
    // as pthread_t is an opaque type
    // as we wouldn't know what type it is
    printf("Hello from thread %lu\n", (unsigned long)pthread_self());

    // To print the thread id itself
    // we can do that using a syscall
    // but it is different from the above print statement
    printf("%d\n", (pid_t)syscall(SYS_gettid));
    return NULL;
}

int main()
{
    // Create the threads
    pthread_t th[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {

        if (pthread_create(&th[i], NULL, f, NULL) != 0)
        {
            perror("Failed to create thread");
            return 1;
        }

        // This should not be done
        // as pthread_t is an opaque type
        // as we wouldn't know what type it is
        printf("Thread (pthread_t): %lu\n", (unsigned long)th[i]);
    }

    // Join the threads
    for (int i = 0; i < NUM_THREADS; i++)
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Failed to join thread");
            return 2;
        }
    return 0;
}