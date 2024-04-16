#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void slaveProcess(int id)
{
    printf("Slave process %d started\n", id);
    // Simulate some work
    sleep(1 + id); // Sleep for id + 1 seconds
    printf("Slave process %d completed\n", id);
}

int main()
{
    int numSlaves = 3; // Number of slave processes

    // Fork multiple slave processes
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
            slaveProcess(i);
            exit(EXIT_SUCCESS);
        }
    }

    // Wait for all slave processes to complete
    for (int i = 0; i < numSlaves; i++) 
    {
        wait(NULL);
    }

    // Master process continues execution
    printf("All slave processes completed\n");

    return 0;
}

