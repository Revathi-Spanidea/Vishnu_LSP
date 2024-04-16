#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_SIZE 1024

typedef struct {
    int data;  // Shared data structure for communication
    // Add more shared data structures as needed
} SharedData;

void slaveProcess(SharedData *sharedData) 
{
    // Perform computation
    int result = sharedData->data * 2;

    // Update shared memory with result
    sharedData->data = result;
}

void slaveProcess1(SharedData *sharedData)
{
	int result=sharedData->data *3;

	sharedData->data=result;
}

int main() 
{
    int shmid;
    key_t key = ftok("shared_memory_key",-1);
    // Generate a key for shared memory The ftok() function returns a key based on path d that is usable in subsequent calls to msgget(), semget(), and shmget()

    // Create shared memory segment
    key_t key1=ftok("shared",-1);

    if ((shmid = shmget(key, sizeof(SharedData), IPC_CREAT | 0666)) < 0) 
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    else
    {
	    printf("Shared memeory segment creation successfull\n");
    }
            if ((shmid = shmget(key1, sizeof(SharedData), IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    else
    {
            printf("memory is re-written\n");
    }



    // Attach shared memory segment
    SharedData *sharedData = (SharedData *)shmat(shmid, NULL, 0);
    if ((intptr_t)sharedData == -1) 
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    else
    {
	    printf("Shared memory segment attachment success\n");
    }
      


    // Initialize shared data
    sharedData->data = 1;  // Example initial data

    // Fork multiple slave processes
    int numSlaves = 3; // Example: Create 3 slave processes
    for (int i = 0; i < numSlaves; i++) 
    {
        pid_t pid = fork();
        if (pid < 0)
       	{
            perror("fork");
            exit(EXIT_FAILURE);
        }
       	else if (pid == 0) 
	{
            // Child process (slave)
            slaveProcess(sharedData);
            exit(EXIT_SUCCESS);
        }
    }
        for (int i = 0; i < numSlaves; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Child process (slave)
            slaveProcess1(sharedData);
            exit(EXIT_SUCCESS);
        }
    }


    // Wait for all slave processes to complete
    for (int i = 0; i < numSlaves; i++) 
    {
        wait(NULL);
    }

    // Print final result from shared memory
    printf("Final result after parallel processing: %d\n", sharedData->data);
        for (int i = 0; i < numSlaves; i++)
    {
        wait(NULL);
    }

    // Print final result from shared memory
    printf("Final result after parallel processing: %d\n", sharedData->data);


    // Detach and delete shared memory segment
    if (shmdt(sharedData) == -1) 
    {
        perror("shmdt");
    }
    else
    {
	    printf("Shared memory segment detach was success\n");
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) 
    {
        perror("shmctl");
    }
    if (shmdt(sharedData) == -1)
    {
        perror("shmdt");
    }
    else
    {
            printf("Shared memory segment detach was success\n");
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
    }

    return 0;
}

