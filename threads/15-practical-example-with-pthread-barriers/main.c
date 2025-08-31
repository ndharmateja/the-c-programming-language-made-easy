#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Objective:
// Every thread rolls a dice, saves its value in an array
// The main thread calculates the winner
// Each thread prints a message with whether or not they lost or won

// Solution
// 1. We use pthread barriers to synchronize the threads
// 2. We have two barriers, one for the dice rolls and one for the status update
// 3. We can't have the main thread calculate the max and update the statuses
// before the dice values are calculated by the threads
// 4. So we wait for the dice roll barrier in each of the 8 threads
// after calculating the dice values and we wait for the same
// dice roll barrier in the main thread before updating the statuses
// so that the main thread waits until all the 8 threads rolled the dice
// 5. Now the issue is that each of the 8 threads have to wait for the main
// thread to update the statuses before printing the status results
// 6. So we synchronize again, this time with the status update barrier (in
// each of the 8 threads and in the main thread after the status update)
// 7. Now we can print the results in each of the threads

// Diagram of the flow:
//                threads  0 1 2 3 4 5 6 7 8       main
//                         | | | | | | | | |        |
//                             roll dice            |
//                         | | | | | | | | |        |
//                         v v v v v v v v v        v
//      roll dice barrier --------------------------------------
//                         | | | | | | | | |        |
//                         | | | | | | | | |  calculate max
//                         | | | | | | | | |  & update statuses
//                         | | | | | | | | |        |
//                         v v v v v v v v v        v
//  status update barrier --------------------------------------
//                         | | | | | | | | |
//                         v v v v v v v v v
//                           print results

// Array to store the dice values
// Array to store if a thread has won
#define NUM_THREADS 8
int dice_values[NUM_THREADS];
int status[NUM_THREADS] = {0};

// Barriers for the two synchronization points
pthread_barrier_t barrier_roll_dice;
pthread_barrier_t barrier_status_update;

void *roll_dice(void *arg)
{
    int index = *(int *)arg;
    dice_values[index] = rand() % 6 + 1;

    // Here we wait for all the threads to synchronize before the main thread
    // can calculate the max and update the statuses
    pthread_barrier_wait(&barrier_roll_dice);

    // We have to wait again for the main thread to update the status
    // so that we print the status after the main thread has updated it
    // This barrier is crossed only after all the 8 threads reach here
    // and the main thread has updated the statuses
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
    // This barrier is crossed only after all the 8 threads roll the dice
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