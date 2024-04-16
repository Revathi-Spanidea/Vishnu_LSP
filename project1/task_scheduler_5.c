#include <stdio.h> //standard input/output operations
#include <stdlib.h> //memory allocation
#include <unistd.h> //process control
#include <pthread.h> //thread management

#define NUM_TASKS 10
#define NUM_SLAVES 3

pthread_mutex_t lock; //declare a mutex lock and a variable to keep track of the number of tasks completed. 
int tasks_completed = 0;

void* slave(void* arg)
{
    int id = *((int*)arg); //converts the void* pointer to an int* pointer, *((int*)arg) retrieves the integer value stored at that memory address.

    while (1) //slave process enters a loop to continuously check for tasks
    {
        pthread_mutex_lock(&lock); // locks the mutex to ensure mutual exclusion and prevent race conditions.
        if (tasks_completed >= NUM_TASKS)
       	{
            pthread_mutex_unlock(&lock);  //the slave releases the mutex lock and exits the loop.
            break;
        }

        printf("Slave %d is working on task %d\n", id, tasks_completed);  // the slave prints the task it's working on
        tasks_completed++;
        pthread_mutex_unlock(&lock);
        // Simulates task execution time by sleeping for a random duration between 1 to 3 seconds.
        sleep(1);
    }

    printf("Slave %d has finished its tasks\n", id);
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_SLAVES];
    int slave_ids[NUM_SLAVES];

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < NUM_SLAVES; i++) 
    {
        slave_ids[i] = i;
        pthread_create(&threads[i], NULL, slave, &slave_ids[i]);
    }

    for (int i = 0; i < NUM_SLAVES; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    printf("All tasks completed\n");

    return 0;
}

