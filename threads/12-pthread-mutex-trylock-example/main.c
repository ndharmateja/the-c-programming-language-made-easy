#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>

// chefs are threada
// stove is shared data (+ mutex)
// cooking happens in the f function
// Let us say there are 4 stoves

// We need 4 mutexes because there are 4 stoves
pthread_mutex_t stove_mutex[4];
int stove_fuel[] = {100, 100, 100, 100};

void *f(void *arg)
{
    // We can use one of the free stoves
    // We can do that by using pthread_mutex_trylock as if one of the stoves
    // is not free, we can try the next one and so on

    // Keep looking for stoves if none of them are free
    // We can use an infinite loop here because once we find a free stove
    // we exit the function, so we can use an infinite loop
    while (true)
    {
        // Try each of the 4 stoves for a free one
        for (int i = 0; i < 4; i++)
        {
            // We try to lock the ith stove
            // the function returns 0 if the lock was successful
            // so if it wasn't successful we simply go to the next stove
            if (pthread_mutex_trylock(&stove_mutex[i]) != 0)
                continue;

            // If there is not enough fuel needed, we simply exit
            int fuel_needed = rand() % 30;
            if (stove_fuel[i] < fuel_needed)
            {
                printf("[stove %d] Not enough stove fuel. Fuel needed: %d, Stove fuel: %d.\n", i, fuel_needed, stove_fuel[i]);
            }

            // If there is enough fuel, we use it
            else
            {
                usleep(500000);
                stove_fuel[i] -= fuel_needed;
                printf("[stove %d] Used %d units of stove fuel. Remaining stove fuel: %d\n", i, fuel_needed, stove_fuel[i]);
            }

            // Unlock the stove
            pthread_mutex_unlock(&stove_mutex[i]);

            // We return because we only want each chef to cook only once
            // on each stove
            return NULL;
        }

        // We reach here if none of the stoves were free
        // So now we can wait for some time and try again
        // This way we don't use up all the CPU time by running an infinite loop
        printf("No stove is free. Waiting...\n");
        usleep(300000);
    }

    return NULL;
}

int main()
{
    // Random seed
    srand(time(NULL));

    // Initialize the stove mutexes
    for (int i = 0; i < 4; i++)
        pthread_mutex_init(&stove_mutex[i], NULL);

    // Create 10 chefs
    pthread_t chefs[10];
    for (int i = 0; i < 10; i++)
        pthread_create(&chefs[i], NULL, f, NULL);

    // Join the threads and destroy the mutex
    for (int i = 0; i < 10; i++)
        pthread_join(chefs[i], NULL);

    // Destroy the stove mutexes
    for (int i = 0; i < 4; i++)
        pthread_mutex_destroy(&stove_mutex[i]);

    // Print the stove fuel
    printf("Stove fuels: [%d, %d, %d, %d]\n", stove_fuel[0], stove_fuel[1], stove_fuel[2], stove_fuel[3]);
    return 0;
}