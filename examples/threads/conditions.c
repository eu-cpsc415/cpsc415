/*
Example of using pthread conditions.

To compile:
gcc conditions.c -o conditions

Multiple threads can wait on a condition.

- pthread_cond_signal() will signal only one waiting thread.
- pthread_cond_broadcast() will signal *all* waiting threads.
*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ready = 0;

// Entry point for the waiter thread
void* waiter(void* arg)
{
    // Get id value from arguments
    int id = *(int*)arg;

    // Acquire lock
    pthread_mutex_lock(&lock);

    while (!ready) {
        // Wait for the condition to be signaled
        printf("Thread %i waiting.\n", id);
        pthread_cond_wait(&cond, &lock);
    }

    printf("waiter %d: got signal\n", id);

    // Release lock
    pthread_mutex_unlock(&lock);

    // End of thread
    return NULL;
}

int main(void)
{
    pthread_t threads[3];
    int ids[3] = {0, 1, 2};

    // Create three new threads
    for (int i = 0; i < 3; i++)
        pthread_create(&threads[i], NULL, waiter, &ids[i]);

    // Sleep main thread for 1 second (give time for threads to start and wait)
    sleep(1);

    // Demo that pthread_cond_signal only wakes one waiting thread
    printf("main: sending signal (only one waiter should wake)\n");
    pthread_mutex_lock(&lock);
    ready = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);

    sleep(1);
    printf("main: only one thread woke up, the other two are still sleeping\n");

    // Broadcast to wake the remaining two
    printf("main: broadcasting to wake remaining waiters\n");
    pthread_mutex_lock(&lock);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);

    // Wait for threads to exit
    for (int i = 0; i < 3; i++)
        pthread_join(threads[i], NULL);

    return 0;
}
