#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 5  //Define constants like NUM_THREADS to specify the number of threads.

int shared_resource = 0;  // Declare a global variable shared_resource 
sem_t semaphore;  // declare a semaphore variable semaphore.

void* thread_function(void* arg)
{
    int id = *((int*)arg); //converts the void* pointer to an int* pointe, *((int*)arg) retrieves the integer value stored at that memory address.
    
    // Wait on the semaphore,If the semaphore value is greater than 0, it decrements the semaphore value and continues. Otherwise, it blocks until the semaphore value becomes positive.
    sem_wait(&semaphore);
    
    printf("Thread %d is accessing the shared resource.\n", id);
    shared_resource++;
    printf("Shared resource value after increment by Thread %d: %d\n", id, shared_resource); // Threads will increment the shared_resource variable while ensuring mutual exclusion using the semaphore.
    
    // Release the semaphore incrementing its value.
    sem_post(&semaphore);
    
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Initialize the semaphore with an initial value of 1, indicating that only one thread can access the shared resource at a time.
    sem_init(&semaphore, 0, 1); // Initial value is 1,The pshared argument(2nd arguments) has a non-zero value, then the semaphore is shared between processes;
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) 
    {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]) != 0) 
	{
            fprintf(stderr, "Error creating thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy semaphore
    sem_destroy(&semaphore);

    return 0;
}

