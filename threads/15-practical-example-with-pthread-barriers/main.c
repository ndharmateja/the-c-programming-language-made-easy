#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Objective:
// Every thread rolls a dice, saves its value in an array
// The main thread calculates the winner
// Each thread prints a message with whether or not they lost or won

#define NUM_THREADS 8

// Array to store the dice values
// Array to store if a thread has won
// Barrier
int dice_values[NUM_THREADS];
int status[NUM_THREADS] = {0};
pthread_barrier_t barrier_roll_dice;
pthread_barrier_t barrier_status_update;

void *roll_dice(void *arg)
{
    int index = *(int *)arg;
    dice_values[index] = rand() % 6 + 1;

    // The if condition has to run after the main thread updates the status
    // Here we wait for all the threads to synchronize before the main thread
    // can calculate the max and update the statuses
    pthread_barrier_wait(&barrier_roll_dice);

    // We have to wait again for the main thread to update the status
    // so that we print the status after the main thread has updated it
    pthread_barrier_wait(&barrier_status_update);

    // Print the result based on the status
    if (status[index])
        printf("(thread %d rolled %d) I won!\n", index, dice_values[index]);
    else
        printf("(thread %d rolled %d) I lost!\n", index, dice_values[index]);

    // Free the dynamically allocated memory
    free(arg);

    return NULL;
}

int main()
{
    // Random seed
    srand(time(NULL));

    // Initialize the barriers
    // We use NUM_THREADS + 1 because we need to consider the main thread as well
    pthread_barrier_init(&barrier_roll_dice, NULL, NUM_THREADS + 1);
    pthread_barrier_init(&barrier_status_update, NULL, NUM_THREADS + 1);

    // Create threads
    pthread_t th[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        int *i_ptr = malloc(sizeof(int));
        *i_ptr = i;
        if (pthread_create(th + i, NULL, roll_dice, i_ptr) != 0)
        {
            perror("Failed to create thread");
            return 1;
        }
    }

    // We wait for the roll dice barrier here as it is at this point
    // we need to have the dice rolls calculated
    // before we can find the max and update the statuses
    pthread_barrier_wait(&barrier_roll_dice);

    // Find the max among the dice values
    int max = 0;
    for (int i = 0; i < NUM_THREADS; i++)
        max = dice_values[i] > max ? dice_values[i] : max;

    // Update the winning status
    for (int i = 0; i < NUM_THREADS; i++)
        status[i] = dice_values[i] == max;

    // Here we wait for the status update barrier
    // to ensure that the status is updated before each of
    // the threads print their results
    pthread_barrier_wait(&barrier_status_update);

    // Join the threads
    for (int i = 0; i < NUM_THREADS; i++)
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Failed to join thread");
            return 2;
        }

    // Destroy the barrier
    pthread_barrier_destroy(&barrier_roll_dice);
    pthread_barrier_destroy(&barrier_status_update);

    return 0;
}