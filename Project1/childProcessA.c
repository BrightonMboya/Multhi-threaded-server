#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    // Check if the number of processes in the chain was specified as a command-line argument
    if (argc != 2)
    {
        printf("Usage: %s <num_processes>\n", argv[0]);
        return 1;
    }

    // Number of child processes to create
    int num_child_processes = atoi(argv[1]);

    // Create the child processes
    for (int i = 0; i < num_child_processes; i++)
    {
        pid_t pid = fork();

        // Check if the fork was successful
        if (pid < 0)
        {
            // Fork failed
            perror("Fork failed");
            return 1;
        }
        else if (pid == 0)
        {
            // This is the child process
            printf("Hello from child process %d\n", getpid());
            return 0;
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_child_processes; i++)
        wait(NULL);

    return 0;
}
