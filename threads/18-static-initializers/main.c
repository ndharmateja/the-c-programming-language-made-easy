#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// If we want to create a mutex with the second argument as NULL
// we can use the static initializer
// Then we don't have to call pthread_mutex_init(&mutex, NULL)
// We can't use this static initializers if we want to create
// an array of them, we would have to loop through them and then
// we can use the PTHREAD_MUTEX_INITIALIZER
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Similarly, if we want to create a condition variable with the second
// argument as NULL, we can use the static initializer
// pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int mails = 0;
#define TEN_MILLION 10000000

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
    // Create threads
    pthread_t t1, t2;
    if (pthread_create(&t1, NULL, &f, NULL) != 0 ||
        pthread_create(&t2, NULL, &f, NULL) != 0)
        return 1;

    if (pthread_join(t1, NULL) != 0 || pthread_join(t2, NULL) != 0)
        return 2;

    // Destroy mutex
    pthread_mutex_destroy(&mutex);

    printf("Mails: %d\n", mails);
    return 0;
}