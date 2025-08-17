#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

// Represents the amount of fuel at the fuel station
int fuel = 0;

// Mutex for locking the fuel as it is accessed by multiple threads
pthread_mutex_t mutex_fuel;

// Condition variable for waiting
// We can do three things with the condition variable
// 1. Wait - p_thread_cond_wait
// 2. Signal - p_thread_cond_signal
// 3. Broadcast - p_thread_cond_broadcast
pthread_cond_t cond_fuel;

void *fuel_filling()
{
    // For filling the fuel in the fuel station itself
    // in 5 rounds. In each round the fuel station will be filled with
    // 15 units of fuel
    // The amount of fuel in the fuel station initially is 0 units
    printf("[station] Fuel filling started\n");
    for (int i = 0; i < 5; i++)
    {
        // Fill 15 units of fuel in each round
        // We also lock the mutex before and unlock it after to
        // avoid race conditions
        pthread_mutex_lock(&mutex_fuel);
        fuel += 15;
        printf("[station] Filled fuel: %d -> %d units\n", fuel - 15, fuel);
        pthread_mutex_unlock(&mutex_fuel);

        // Signal the waiting threads after ever 15 units is filled
        // in the station
        // Does not do anything to the mutex
        // Simply sends signals to all the waiting threads
        pthread_cond_signal(&cond_fuel);

        // Wait for 3 seconds before filling the next round of 15 units
        // in the fillings station
        sleep(3);
    }
    return NULL;
}

bool has_enough_fuel(int required_fuel)
{
    return fuel >= required_fuel;
}

void *car()
{
    // The car needs 40 units of fuel in its tank
    // So when the fuel is 0 initially, the car can't be filled
    // So the car will have to wait until the fuel station has
    // atleast 40 units of fuel
    // Condition variables can be used to do this
    // If we didn't have a condition variable, our loop would have been inside
    // the locked region and therefore it would have been stuck there
    // as it won't be unlocked until fuel reaches >= 40
    // But the fuel won't get started filling in the station as this thread
    // locked the mutex
    printf("[car] Here to get fuel\n");
    pthread_mutex_lock(&mutex_fuel);
    while (!has_enough_fuel(40))
    {
        printf("[car] Current fuel: %d units. Not enough fuel. Waiting...\n", fuel);

        // 1. As there is not enough fuel, we wait
        // 2. And once we get a signal from the other thread after each fuel fill
        // in the station, the pthread_cond_wait continues and therefore checks the while condition
        // to see if there is enough fuel and if there isn't, it will do the
        // conditional wait again
        // 3. Once we get a conditional signal and there is indeed enough fuel
        // we exit the while loop (as loop condition fails) and actually do the filling
        // 4. When we call the pthread_cond_wait, it will unlock the mutex (that is why we are
        // passing the mutex to the function) and therefore the fuel_filling fn
        // can continue running (as it was waiting for a locked mutex)
        // and when the fuel is filled, it will signal the car function
        // and the mutex will be locked again here

        // So the below code is equivalent to:
        // pthread_mutex_unlock(&mutex_fuel);
        // <wait for the signal on cond_fuel>
        // pthread_mutex_lock(&mutex_fuel);
        pthread_cond_wait(&cond_fuel, &mutex_fuel);
    }

    // Fill the fuel and unlock the mutex
    fuel -= 40;
    printf("[car] Got 40 units of fuel. Remaining fuel: %d units.\n", fuel);
    pthread_mutex_unlock(&mutex_fuel);

    // Exit
    return NULL;
}

int main()
{
    // Initialize mutex anch condition variable
    pthread_mutex_init(&mutex_fuel, NULL);
    pthread_cond_init(&cond_fuel, NULL);

    // Create the threads
    pthread_t t0, t1;
    pthread_create(&t0, NULL, &car, NULL);
    pthread_create(&t1, NULL, &fuel_filling, NULL);

    // Join the threads
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    // Destroy mutex and condition variable
    pthread_mutex_destroy(&mutex_fuel);
    pthread_cond_destroy(&cond_fuel);

    // Exit
    return 0;
}