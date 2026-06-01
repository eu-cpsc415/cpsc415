/*
Example of creating multiple threads using pthreads.

To compile:
gcc pthreads.c -o pthreads

With optimizations:
gcc pthreads.c -O2 -o pthreads
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 4

// This function is used as the entry point for each new thread,
// specified in the call to pthread_create().
void* thread_run(void* arg)
{
    int id = *(int*)arg;
    printf("Thread %d starting\n", id);

    // Simulate the thread doing work
    sleep(NUM_THREADS - id + 1);

    printf("Thread %d done\n", id);
    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_THREADS]; // Array of thread handles
    int ids[NUM_THREADS];           // Array of thread IDs to pass into each thread as an argument

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, thread_run, &ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
