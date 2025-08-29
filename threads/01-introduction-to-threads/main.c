#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// We have to link with -pthread while compiling

void *f()
{
    printf("Hello from thread\n");
    sleep(1);
    printf("Goodbye from thread\n");

    return NULL;
}

int main()
{
    // Create threads
    // Do error handling as some linux envs don't allow for
    // thread creations due to resource management
    pthread_t t1, t2;
    if (pthread_create(&t1, NULL, &f, NULL) != 0 || pthread_create(&t2, NULL, &f, NULL) != 0)
        return 1;

    // Join the threads
    // Both threads run in parallel
    if (pthread_join(t1, NULL) != 0 || pthread_join(t2, NULL) != 0)
        return 2;

    return 0;
}