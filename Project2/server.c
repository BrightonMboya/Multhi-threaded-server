#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080              // Port number to use for the server
#define MAX_CLIENTS 10         // Maximum number of clients that can connect to the server
#define MAX_MESSAGE_LENGTH 256 // Maximum length of a message that can be sent by a client

// Data structure to store information about a client
struct client_info
{
    int sock_fd;      // Socket file descriptor for the client
    pthread_t thread; // Thread for the client
};

void *handle_client(void *arg)
{
    struct client_info *client = (struct client_info *)arg; // Get the client information from the argument
    struct client_info clients[MAX_CLIENTS];

    char message[MAX_MESSAGE_LENGTH]; // Buffer to store the received message

    while (1)
    {
        // Receive a message from the client
        int bytes_received = recv(client->sock_fd, message, MAX_MESSAGE_LENGTH, 0);

        // If the client has closed the connection, break out of the loop
        if (bytes_received == 0)
        {
            break;
        }

        // Broadcast the message to all other clients
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            // Skip the client that sent the message
            if (clients[i].sock_fd == client->sock_fd)
            {
                continue;
            }

            // Send the message to the other client
            send(clients[i].sock_fd, message, bytes_received, 0);
        }
    }

    // Close the socket for the client
    close(client->sock_fd);

    return NULL;
}

int main()
{
    // Create a server socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Set the socket options
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind the socket to a port
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
}
