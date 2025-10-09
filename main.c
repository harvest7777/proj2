// Group Project 2: Synchronization - Dining Philosophers Problem
// CECS 326 - Operating Systems

// Five philosophers , number 0 . . . 4.
// only one thread per philosopher and only thinking or eating at a time
// stack overflow: https://stackoverflow.com/questions/52960662/trying-to-understand-pthread-cond-lock-and-pthread-cond-signal
// stack overflow 2 : https://stackoverflow.com/questions/72094630/what-do-the-pthread-mutex-lock-and-pthread-mutex-unlock-do

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define total_philosophers 5 // Number of philosophers
#define think 0
#define hungry 1
#define eating 2

// Global synchronization variables
pthread_mutex_t mutex;
pthread_cond_t cond[total_philosophers];
int state[total_philosophers];      // Current state of each philosopher
int fork_owner[total_philosophers]; // Track which philosopher owns each fork

// Get left fork
int left_fork(int i)
{
    return i;
}

// Get right fork
int right_fork(int i)
{
    return (i + 1) % total_philosophers;
}

// Get left neighbor philosopher, 0 need fork 0 and 1 to eat
int left_neighbor(int i)
{
    return (i + total_philosophers - 1) % total_philosophers;
}

// Get right neighbor philosopher
int right_neighbor(int i)
{
    return (i + 1) % total_philosophers;
}

// Resources : From professor lecture slide
void test(int i)
{
    if (state[i] == hungry &&
        state[left_neighbor(i)] != eating &&
        state[right_neighbor(i)] != eating)
    {

        state[i] = eating;

        fork_owner[left_fork(i)] = i;
        fork_owner[right_fork(i)] = i;

        printf("Forks are with Philosopher #%d\n", i);
        pthread_cond_signal(&cond[i]); // stack overflow search
    }
}

// show  which philosopher owns fork
void display_forks()
{
    for (int i = 0; i < total_philosophers; i++)
    {
        if (fork_owner[i] != -1)
        {
            printf("  Fork #%dis with %d\n", i, fork_owner[i]);
        }
    }
}

// Called when a philosopher wants to pick up forks and eat
// Resources : From professor lecture slide
void pickup_forks(int philosopher_number)
{
    // Enter critical section
    pthread_mutex_lock(&mutex);

    // Set state to hungry - philosopher wants to eat
    state[philosopher_number] = hungry;

    // Test if we can eat immediately or wait
    test(philosopher_number);

    // If we can't eat (neighbors are eating), wait on condition variable
    while (state[philosopher_number] != eating)
    {
        pthread_cond_wait(&cond[philosopher_number], &mutex);
    }

    pthread_mutex_unlock(&mutex); // stack overflow search
}

void return_forks(int philosopher_number)
{
    // Lock the mutex to enter critical section
    pthread_mutex_lock(&mutex);

    // Set state back to thinking
    state[philosopher_number] = think;
    printf("Philosopher %d: Returning forks and thinking\n", philosopher_number);
    fflush(stdout);

    // Test if neighbors can now eat
    test(left_neighbor(philosopher_number));
    test(right_neighbor(philosopher_number));

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);
}