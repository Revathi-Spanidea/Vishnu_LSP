// Slave Node (Client) Code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081

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
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    else
    {
	    printf("Socket craetion was successful\n");
    }

    // Initialize server address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }
    else
    {
	    printf("Valid address\n");
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }
    else
    {
	    printf("Connection successful from the server\n");
    }

    // Send read request
    Message readMsg;
    readMsg.type = READ_REQUEST;
    readMsg.address = 0; // Example memory address
    if (send(sock, &readMsg, sizeof(readMsg), 0) < 0) {
        perror("failed to send read request");
        exit(EXIT_FAILURE);
    }
    else
    {
	    printf("Successfully send the read request\n");
    }

    // Receive read response
    Message readResponse;
    if (recv(sock, &readResponse, sizeof(readResponse), 0) < 0) {
        perror("failed to receive read response");
        exit(EXIT_FAILURE);
    }
    else
    {
	    // Process read response
	    printf("Successfully received the read response\n");
	    printf("Read response received: data at address %d is %d\n", readResponse.address, readResponse.data);
    }

    // Send write request
    Message writeMsg;
    writeMsg.type = WRITE_REQUEST;
    writeMsg.address = 1; // Example memory address
    writeMsg.data = 42;   // Example data to write
    if (send(sock, &writeMsg, sizeof(writeMsg), 0) < 0) {
        perror("failed to send write request");
        exit(EXIT_FAILURE);
    }
    else
    {
	    printf("Successfull to send write request\n");
    }

    // Receive write response
    Message writeResponse;
    if (recv(sock, &writeResponse, sizeof(writeResponse), 0) < 0) {
        perror("failed to receive write response");
        exit(EXIT_FAILURE);
    }
    else
    {
	    printf("Received write responsed successfully\n");
	    // Process write response
            printf("Write response received: write to address %d was successful\n", writeResponse.address);
    }

    close(sock);

    return 0;
}

