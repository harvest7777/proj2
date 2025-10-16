// Group Project 2: Synchronization - Dining Philosophers Problem
// CECS 326 - Operating Systems

// Five philosophers , number 0 . . . 4.
// only one thread per philosopher and only thinking or eating at a time
// Resources: 
// stack overflow: https://stackoverflow.com/questions/52960662/trying-to-understand-pthread-cond-lock-and-pthread-cond-signal
// stack overflow 2 : https://stackoverflow.com/questions/72094630/what-do-the-pthread-mutex-lock-and-pthread-mutex-unlock-do
// geek for geeks : https://www.geeksforgeeks.org/c/thread-functions-in-c-c/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define N 5 // Num of philosophers

// Philosopher states
#define think 0
#define hungry 1
#define eat 2

// Global synchronization variables
pthread_mutex_t mutex;
pthread_cond_t cond[N]; // For getting notified on fork availability
int state[N];      // Current state of each philosopher
int fork_owner[N]; // Track philosopher owns fork 

// Get current time 
long time_cal()
{
    struct timeval tv;
    gettimeofday(&tv, NULL); // https://stackoverflow.com/questions/5362577/c-gettimeofday-for-computing-time
    return tv.tv_sec * 1000 + tv.tv_usec / 1000; // Convert to milliseconds
}

// Get left fork 
int left_fork(int i)
{
    return i;
}

// Get right fork 
int right_fork(int i)
{
    return (i + 1) % N;
}

// Get left neighbor philosopher, 0 need fork 0 and 1 to eat
int left_neighbor(int i)
{
    return (i + N - 1) % N;
}

// Get right neighbor philosopher
int right_neighbor(int i)
{
    return (i + 1) % N;
}

void print_forks()
{
    for (int i = 0; i < N; i++)
    {
        if (fork_owner[i] != -1)
        {
            printf(" Fork #%dis with %d\n", i, fork_owner[i]);
        }
    }
}

// Resources : From professor lecture slide
void test(int i)
{
    if (state[i] == hungry &&
        state[left_neighbor(i)] != eat &&
        state[right_neighbor(i)] != eat)
    {

        state[i] = eat;

        // Assign forks to this philosopher
        fork_owner[left_fork(i)] = i;
        fork_owner[right_fork(i)] = i;

        printf("    Forks are with Philosopher #%d\n", i);
        // Wakes up the thread that is waiting on this condition is on (the current philospher)
        // this will let the philospher eat
        pthread_cond_signal(&cond[i]); // stack overflow search
    }
}

// Called function when a philosopher wants to pick up forks and eat
// Resources : From professor lecture slide
void pickup_forks(int philosopher_number)
{
    pthread_mutex_lock(&mutex);

    state[philosopher_number] = hungry;
    test(philosopher_number);

    while (state[philosopher_number] != eat)
    {
        // let the other philosphers mutate the cond at this philospher
        pthread_cond_wait(&cond[philosopher_number], &mutex);
    }

    pthread_mutex_unlock(&mutex); // stack overflow search : https://stackoverflow.com/questions/72094630/what-do-the-pthread-mutex-lock-and-pthread-mutex-unlock-do
}

// Return forks
void return_forks(int philosopher_number)
{
    // Lock the mutex to enter critical section

    pthread_mutex_lock(&mutex);

    state[philosopher_number] = think;

    // Release forks
    fork_owner[left_fork(philosopher_number)] = -1;
    fork_owner[right_fork(philosopher_number)] = -1;

    // Test if neighbors can eat now
    test(left_neighbor(philosopher_number));
    test(right_neighbor(philosopher_number));
    
    // Unlock the mutex
    pthread_mutex_unlock(&mutex); // stack overflow search : https://stackoverflow.com/questions/72094630/what-do-the-pthread-mutex-lock-and-pthread-mutex-unlock-do
}

// Philosopher thread function
void *philosopher(void *arg)
{
    int id = *(int *)arg;
    long start_time, elapsed_time;

    while (1)
    {
        // Thinking
        start_time = time_cal();
        int think_time = (rand() % 3) + 1;
        sleep(think_time);
        elapsed_time = time_cal() - start_time;
        printf("Philosopher #%d took %ldms thinking\n", id, elapsed_time);

        // Pick up forks
        pickup_forks(id);

        // Eating
        start_time = time_cal();
        int eat_time = (rand() % 3) + 1;
        sleep(eat_time);
        elapsed_time = time_cal() - start_time;
        printf("Philosopher #%d took %ldms eating\n", id, elapsed_time);

        return_forks(id);// call return forks to release forks

        // Display fork status 
        pthread_mutex_lock(&mutex);
        print_forks();
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main()
{
    pthread_t threads[N];
    int ids[N];

    srand(time(NULL));

    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < N; i++)
    {
        pthread_cond_init(&cond[i], NULL);
        state[i] = think;
        fork_owner[i] = -1; // All forks initially available
        ids[i] = i;
    }

    printf("DiningPhilosophers\n");

    // Display fork status
    for (int i = 0; i < N; i++)
    {
        printf("     Fork #%dis with %d\n", i, (i + N - 1) % N);
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++)
    {
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);// https://www.geeksforgeeks.org/c/thread-functions-in-c-c/

    }

    sleep(40); // I used 40 seconds to allow philosophers to eat and think 

    // Cancel threads
    for (int i = 0; i < N; i++)
    {
        pthread_cancel(threads[i]); 
    }

    // pthread_mutex_destroy to free up resources 
    pthread_mutex_destroy(&mutex); // https://stackoverflow.com/questions/76191268/when-is-pthread-mutex-destroy-necessary
    for (int i = 0; i < N; i++)
    {
        pthread_cond_destroy(&cond[i]);
    }

    return 0;
}