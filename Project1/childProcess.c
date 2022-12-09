#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    // Check if the number of processes in the chain was specified as a command-line argument
    if (argc != 2)
    {
        printf("Usage: %s <num_processes>\n", argv[0]);
        return 1;
    }

    // Get the number of processes in the chain from the command-line argument
    int num_processes = atoi(argv[1]);

    // Create the chain of processes
    for (int i = 0; i < num_processes; i++)
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
            if (i == num_processes - 1)
            {
                // This is the last process in the chain, so print the message and exit
                printf("Hello from process %d\n", getpid());

                return 0;
            }
            else
            {
                // This is not the last process in the chain, so wait for the child process to finish
                wait(NULL);
            }
        }
        else
        {
            // This is the parent process, so wait for the child process to finish
            wait(NULL);
        }
    }

    return 0;
}
