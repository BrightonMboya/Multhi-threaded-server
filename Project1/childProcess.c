#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int childProcess()
{
    int i;
    for (i = 0; i < 3; i++)
    {
        // now let's create the create the child pid
        int child_pid;

        // now check if the child_pid < o that means we got an error and we need to exit
        if ((child_pid = fork()) < 0)
        {
            perror("fork");
            exit(1);
        }
        else if (child_pid > 0)
        {
            printf("This is the process of a Parent, I forked child %d with a pid of %d \n", i, child_pid);
            /* Now from here we continue the process until no child is left and we will wait for a while before exiting*/
        }
        else
        {
            // now here we get the child_pid by calling the getpid() system call
            printf("Hello World, I am the %d child, I have a pid of %d \n", i, getpid());

            // now here let's just sleep for 5 seconds
            sleep(5);
        }

        // now this process that get here is  the parent as children are blocked from the exit block above
        // printf("waiting for all children processes to be completed \n");

        int child_exit_pid;
        while ((child_exit_pid = wait(NULL)) >= 0)
        {
            // just wait until we have an error
            printf("This is child with pid == %d and I am exiting \n", child_exit_pid);
        }
        printf("Parent has now exited!!");
    }
}

int main()
{
    childProcess();
}