#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// these are the semaphore variables
sem_t x, y;
pthread_t tid;
pthread_t writerthreads[100];
pthread_t readerthreads[100];
int readerCount = 0;

// this is the reader function
void *reader(void *param)
{
    // entering the critical section so we need to lock the semaphore
    sem_wait(&x);
    readerCount++;

    // here we check if the client is reading
    if (readerCount == 1)
    {
        sem_wait(&y);
    }

    // leaving the critical section so we unlock the semaphore
    sem_post(&x);

    printf("\n%d reader is inside", readerCount);

    // sleep the connection for a while
    sleep(5);

    // locking the semaphore again because its a critical section
    sem_wait(&x);
    readerCount--;

    if (readerCount == 0)
    {
        sem_post(&y);
    }

    sem_post(&x);

    printf("n%d Reader is now leaving", readerCount + 1);

    // exiting the thread
    pthread_exit(NULL);
}

// this is the writer function
void *writer(void *param)
{
    printf("\n Writer is trying to enter");

    // locking the semaphore
    sem_wait(&y);

    printf("\nWriter has entered");

    // unlock the semaphore
    sem_post(&y);

    printf("\n Writer is now leaving");
    // exiting the thread
    pthread_exit(NULL);
}

// the driver code
int main()
{
    // initialize the variables to be used
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;

    socklen_t addr_size;
    sem_init(&x, 0, 1);
    sem_init(&y, 0, 1);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8989);

    // now here we bind the socket to the address and port number
    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    // here we listen to the socket with 40 max connection requests queued

    if (listen(serverSocket, 50) == 0)
    {
        printf("Listening\n");
    }
    else
    {
        printf("Error\n");
    }

    // Array for thread
    pthread_t tid[60];

    int i = 0;

    while (1)
    {
        addr_size = sizeof(serverStorage);

        // Extract the first
        // connection in the queue
        newSocket = accept(serverSocket,
                           (struct sockaddr *)&serverStorage,
                           &addr_size);
        int choice = 0;
        recv(newSocket,
             &choice, sizeof(choice), 0);

        if (choice == 1)
        {
            // Creater readers thread
            if (pthread_create(&readerthreads[i++], NULL,
                               reader, &newSocket) != 0)

                // Error in creating thread
                printf("Failed to create thread\n");
        }
        else if (choice == 2)
        {
            // Create writers thread
            if (pthread_create(&writerthreads[i++], NULL,
                               writer, &newSocket) != 0)

                // Error in creating thread
                printf("Failed to create thread\n");
        }

        if (i >= 50)
        {
            // Update i
            i = 0;

            while (i < 50)
            {
                // Suspend execution of
                // the calling thread
                // until the target
                // thread terminates
                pthread_join(writerthreads[i++],
                             NULL);
                pthread_join(readerthreads[i++],
                             NULL);
            }

            // Update i
            i = 0;
        }
    }

    return 0;
}