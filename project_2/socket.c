#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define MAX_MSG_SIZE 1024

// Custom communication protocol message types
#define READ_REQUEST 0
#define WRITE_REQUEST 1
#define READ_RESPONSE 2
#define WRITE_RESPONSE 3

// Structure for communication messages
typedef struct {
    int type;
    int address;
    int data;
} Message;

int main() 
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_MSG_SIZE] = {0};

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    else
    {
	    printf("Socket creation was successfull\n");
    }

    // Initialize address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to localhost and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("bind failed\n");
        exit(EXIT_FAILURE);
    }
    else
    {
	    printf("Socket bind successfull\n");
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) 
    {
        perror("listen Failed\n");
        exit(EXIT_FAILURE);
    }
    else
    {
	    printf("Listen for connection\n");
    }

    // Accept incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) 
    {
        perror("accept is failed\n");
        exit(EXIT_FAILURE);
    }
    else
    {
	    printf("Accept the incoming connection\n");
    }

    // Handle communication with client
    while (1)
    {
        // Receive message from client
        Message msg;
        if (recv(new_socket, &msg, sizeof(msg), 0) < 0) 
	{
            perror("Failed to read from client\n");
            exit(EXIT_FAILURE);
        }
	else
	{
		printf("Received message from the client\n");
	}

        // Process message based on type
        switch (msg.type) 
	{
            case READ_REQUEST:
                // Handle read request
                printf("Received read request for address %d\n", msg.address);
                // Read data from shared memory and send response
                break;
            case WRITE_REQUEST:
                // Handle write request
                printf("Received write request for address %d with data %d\n", msg.address, msg.data);
                // Write data to shared memory and send response
                break;
            default:
                // Invalid message type
                printf("Invalid message type\n");
        }
    }

    close(new_socket);
    close(server_fd);

    return 0;
}

