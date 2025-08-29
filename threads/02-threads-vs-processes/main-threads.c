#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int x = 2;

// We can have multiple threads in the same process
// It means that all threads have a common address space and they
// can access all the variables in the process
void *f()
{
    x++;
    sleep(1);
    printf("[%d] Hello from threads! x: %d\n", getpid(), x);
    return NULL;
}

void *f2()
{
    sleep(2);
    printf("[%d] Hello from threads! x: %d\n", getpid(), x);
    return NULL;
}

int main()
{
    // Create threads
    pthread_t t1, t2;
    if (pthread_create(&t1, NULL, &f, NULL) != 0 ||
        pthread_create(&t2, NULL, &f2, NULL) != 0)
        return 1;

    // Join the threads
    if (pthread_join(t1, NULL) != 0 || pthread_join(t2, NULL) != 0)
        return 2;

    return 0;
}