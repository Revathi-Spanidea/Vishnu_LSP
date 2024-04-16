#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to perform some task
void perform_task(int process_id) {
    printf("Process %d is executing.\n", process_id);
}

int main() {
    int num_processes = 5;
    int priorities[] = {2, 1, 3, 2, 1}; // Priority values, higher value means higher priority

    // Forking multiple child processes
    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            // Perform task based on priority
            perform_task(i);
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process waits for all child processes to finish
    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    printf("All processes have finished executing.\n");

    return 0;
}

