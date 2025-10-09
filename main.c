
// Group Project 2: Synchronization - Dining Philosophers Problem
// CECS 326 - Operating Systems

// Five philosophers , number 0 . . . 4.
// only one thread per philosopher and only thinking or eating at a time

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define total_philosophers 5 // Number of philosophers
#define think 0
#define hungry 1
#define eating 2

pthread_mutex_t mutex;
pthread_cond_t cond[total_philosophers];
int state[total_philosophers];