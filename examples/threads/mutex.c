/*
Example of using a pthread mutex.

To compile:
gcc mutex.c -o mutex
*/

#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 4
#define NUM_INCREMENTS 100000

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int counter_safe = 0;
int counter_unsafe = 0;

void* increment_safe(void* arg)
{
    for (int i = 0; i < NUM_INCREMENTS; i++) {
        // Acquire the lock before touching the shared counter.
        // Any other thread that tries to lock will block here until
        // we call unlock, ensuring only one thread increments at a time.
        pthread_mutex_lock(&lock);
        counter_safe++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* increment_unsafe(void* arg)
{
    for (int i = 0; i < NUM_INCREMENTS; i++) {
        // counter++ looks atomic but compiles to three separate instructions:
        // read, increment, write. If two threads interleave here, both can
        // read the same value, increment it, and write back the same result,
        // silently losing one increment.
        counter_unsafe++;
    }
    return NULL;
}

void run_test(void* (*func)(void*), const char* label)
{
    pthread_t threads[NUM_THREADS];

    // Spin up all threads and let them race on the counter
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, func, NULL);

    // Wait for all threads to finish before reading the counter
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("%s: expected %d, got %d %s\n",
           label,
           NUM_THREADS * NUM_INCREMENTS,
           label[0] == 'S' ? counter_safe : counter_unsafe,
           label[0] == 'S' ? "(correct)" : "(likely wrong)");
}

int main(void)
{
    run_test(increment_safe, "Safe   (mutex)");
    run_test(increment_unsafe, "Unsafe (no mutex)");
    return 0;
}