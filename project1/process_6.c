#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_TASKS 50
#define NUM_SLAVES 2

pthread_mutex_t lock;  //Acquires the mutex lock to ensure exclusive access to shared resources.
int tasks_completed = 0; //Keeps track of the number of tasks completed.

void* slave(void* arg) 
{
    int id = *((int*)arg);
    while (1) 
    {
        pthread_mutex_lock(&lock); //Mutex lock for synchronization.
        if (tasks_completed >= NUM_TASKS) 
	{
            pthread_mutex_unlock(&lock); //Releases the mutex lock before breaking out of the loop.
            break;
        }
        printf("Slave %d is working on task %d\n", id, tasks_completed); //task being processed by the slave thread.
        tasks_completed++; //Releases the mutex lock after completing the task processing.
        pthread_mutex_unlock(&lock);//Releases the mutex lock after completing the task processing.
    }
    printf("Slave %d has finished its tasks\n", id);
    pthread_exit(NULL);
}

int main() 
{
    pthread_t threads[NUM_SLAVES];
    struct timeval start, end; // structure that represents a duration of time, containing two members: tv_sec for seconds and tv_usec for microseconds.
    double execution_time;

    pthread_mutex_init(&lock, NULL);  // Initializes the mutex lock lock.
    // This function retrieves the current time since the Epoch (00:00:00 Coordinated Universal Time (UTC), January 1, 1970) and stores it in the provided struct timeval.
    gettimeofday(&start, NULL);  //to get the current time from the system clock, &start: This is the address of the struct timeval variable where the current time will be stored.
    //NULL: This parameter is used to specify the timezone. In this case, since it's set to NULL, the function will use the default timezone.
    for (int i = 0; i < NUM_SLAVES; i++) 
    {
        int* id = (int*)malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, slave, (void*)id);
    }

    for (int i = 0; i < NUM_SLAVES; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);

    pthread_mutex_destroy(&lock);

    execution_time = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 50;
    printf("Total execution time: %.6f seconds\n", execution_time);

    return 0;
}

