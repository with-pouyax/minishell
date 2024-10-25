- in last lesson we looked at how to manage multiple pipes, by creating a simple program that passing around 3 processee, a number that was incremented by each.
- now in taht lesson, I didnt look at creating this program in such a way that we can, make as many processes that we want, it was only limited to 3 and it was kind of hardcoded.
- so imagine we have want to create n processors, main send the value to the first process and it increments it by 1 and sends it to the second process and it will continue until it reaches the last process and then it will send it back to the main process.
- first thing first, how many pipes do we need ?
- imagine instead of n processes, we have 3 processes, in this case we need 4 pipes, which is number of processes + 1.
- so for n processes, we need n + 1 pipes.


```c

#define PROCESSES_NUMB 3

int main(int argc, char *argv[])
{
    int pids[PROCESSES_NUMB];
    int pipes[PROCESSES_NUMB + 1][2];
    int i;
    for (i = 0; i < PROCESSES_NUMB + 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            return 1;
        }
    }

    for (i = 0; i < PROCESSES_NUMB; i++)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork");
            return 2;
        }
        if (pids[i] == 0)
        {
            break; // we break out of the loop, so we dont fork anymore in the child process every time.
            /*if I return here instead of break, it will terminate the child process, before it can do anything. */
        }
        /* I can write here wait(NULL), because i am waiting for the child process to finish,
        before I am creating another child process. */

        for (i = 0; i < PROCESSES_NUMB; i++)
        {
            wait(NULL);
        }
    }
}
```

- now we reached the harder part, which pipe do we use, how do we use the pipes and which ones do we close ? which one do we not close ?
- I explained in the way how we can manage the pipes in the diagram file.
- the most important thing is that, each process closes actually all the other pipes, that we have created,
- so in our graph example because we have 3 processes, we have created 4 pipes, but every
single pipe has been copied for every single child process, so all in all we have 4 * 4 number file descriptors open.
- so we have to close most of them.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROCESS_NUM 10 // we set the number of processes to 10

int main(int argc, char* argv[]) 
{
    int pids[PROCESS_NUM]; // we create an array with the size of the number of processes
    int pipes[PROCESS_NUM + 1][2]; /* we create an array of pipes, with the size of the number of processes + 1, +1 because we need to send the value back
    to the main process, and this extra pipe is for that. */
    int i;
    for (i = 0; i < PROCESS_NUM + 1; i++) // in a loop we our pipes
    {
        if (pipe(pipes[i]) == -1)
        {
            printf("Error with creating pipe\n");
            return 1;
        }
    }
    // now we create the processes

    for (i = 0; i < PROCESS_NUM; i++) // in a loop we create the processes 
    {
        pids[i] = fork(); // in each iteration we create a new process
        if (pids[i] == -1) // check for errors
        {
            printf("Error with creating process\n");
            return 2;
        }
        if (pids[i] == 0) // if we are in the child process
        {
            // Child process
            int j;
            for (j = 0; j < PROCESS_NUM + 1; j++) // we have to close all the pipes, except the one that we are using.
            {
                if (i != j) // i here represents the current process and j represents the pipe that we are closing, so if they are not the same, 
                // we close the pipe
                {
                    close(pipes[j][0]);
                }
                if (i + 1 != j) 
                {
                    close(pipes[j][1]);
                }
            }
            int x;
            if (read(pipes[i][0], &x, sizeof(int)) == -1) {
                printf("Error at reading\n");
                return 3;
            }
            printf("(%d) Got %d\n", i, x);
            x++;
            if (write(pipes[i + 1][1], &x, sizeof(int)) == -1) {
                printf("Error at writing\n");
                return 4;
            }
            printf("(%d) Sent %d\n", i, x);
            close(pipes[i][0]);
            close(pipes[i + 1][1]);
            return 0;
        }
    }

    // Main process
    int j;
    for (j = 0; j < PROCESS_NUM + 1; j++) {
        if (j != PROCESS_NUM) {
            close(pipes[j][0]);
        }
        if (j != 0) {
            close(pipes[j][1]);
        }
    }
    int y = 5;
    printf("Main process sent %d\n", y);
    if (write(pipes[0][1], &y, sizeof(int)) == -1) {
        printf("Error at writing\n");
        return 4;
    }
    if (read(pipes[PROCESS_NUM][0], &y, sizeof(int)) == -1) {
        printf("Error at reading\n");
        return 3;
    }
    printf("The final result is %d\n", y);
    close(pipes[0][1]);
    close(pipes[PROCESS_NUM][0]);

    for (i = 0; i < PROCESS_NUM; i++) {
        wait(NULL);
    }
    return 0;
}
```