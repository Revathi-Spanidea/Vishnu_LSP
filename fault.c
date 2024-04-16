#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 5

// Shared data structure
int shared_data = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to simulate processing
void process_data(int thread_id)
{
    sleep(1); // Simulate processing time
    shared_data++;
    printf("Thread %d processed data: %d\n", thread_id, shared_data);
}

// Function executed by each thread
void *thread_function(void *arg) 
{
    int thread_id = *((int *)arg);
    
    // Access shared data under mutex lock for fault tolerance
    pthread_mutex_lock(&mutex);
    process_data(thread_id);
    pthread_mutex_unlock(&mutex);
    
    pthread_exit(NULL);
}

int main() 
{
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_function, (void *)&thread_ids[i]) != 0) 
	{
            fprintf(stderr, "Error creating thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) 
    {
        if (pthread_join(threads[i], NULL) != 0) 
	{
            fprintf(stderr, "Error joining thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}

