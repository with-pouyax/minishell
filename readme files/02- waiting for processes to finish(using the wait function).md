# Waiting for Processes to Finish (Using the wait Function)
- We want to see how to actually wait for processes to finish.
- Let's first create a simple program.
- I want to call fork() only once, and I want to print all numbers from 1 to 10.
- First, I want to print the first 5 numbers in the child process, and then the other 5 numbers in the main process.
- To do that, first I initialize n, which is going to be the number that I start at.
- Then we have to check if we are in the main process because in the main process it will start from 6 to 10, and if we are in the child process, it will start from 1 to 5.
- Then we execute a for loop that goes through all the numbers and prints them. 
- Every time we print something to standard output, it has an internal buffer, so if we print a lot of things very quickly, it is not going to automatically print them on the course, it is going to get all the input and then print it all at once.
- So in our case, I want to print them exactly when I call printf(), so we have to use the fflush() function, so now every time we print a number or a digit, it will be shown immediately on the terminal.
- In the given code, the fflush(stdout) ensures that the output buffer is flushed, meaning that any data stored in the buffer is immediately printed to the terminal. Let's break it down:
- Buffered Output: When you use printf(), the output is typically stored in a buffer (a temporary storage area) and is not immediately sent to the terminal.
- This allows the system to optimize performance by reducing the number of actual writes to the terminal (or output device). However, when the buffer is full, or when the program ends, the buffer gets flushed automatically, and the contents are printed.
- Without fflush(stdout): If you remove the fflush(stdout) line, the program might not immediately print the numbers when printf() is called, especially if the buffer is not full. The data might remain in the buffer until the buffer fills up or the program finishes,
which means the output could be delayed or might appear all at once.

**In This Case:**
- The code creates a child and a parent process using fork().
- Both processes print 5 numbers (the child prints from 1 to 5, and the parent from 6 to 10).

**If fflush(stdout) is removed:**
    Inconsistent output order:
        - Since fork() creates two processes, the output might be mixed between them.
        - Without fflush(stdout), the buffered output from both processes may get printed together or out of order, depending on when the buffers get flushed.
    **Potential delay:**
        - The output might not appear immediately on the terminal but could be delayed until the program exits or the buffer fills up.
        - With fflush(stdout), the output happens immediately, so you'll see each number printed as soon as it is generated.
 ```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h> // Include this header for wait()

int main(int argc, char *argv[])
{
    int id = fork();
    int n;
    if (id == 0)
    {
        n = 1;
    }
    else
    {
        n = 6;
    }

    int i;
    for (i = n; i < n + 5; i++)
    {
        printf("%d\n", i);
        fflush(stdout); // to print the number immediately
    }
    return 0;
}
```

- Now if I run this program, the result will be: 

`1 6 2 7 3 8 4 9 5 10`
- Why? 
- Because at the beginning, we have fork(), which splits the process into two processes. Now both execute every single statement from now on, almost at the same time, so the first process will print 1, then the second process will print its first number, which is 6, then the first process will print 2 and the second process will print 7, and so on.
- So now, how can we receive first the first 5 numbers, and then the other 5 numbers?
- To do that, we have to use the wait() function.
- What the wait() function does is that it stops the execution until a child process finishes executing.
- So right now, we have two processes, we have the main process and the child process, and they are almost executing at the same time, so what we can do is in the main process, we can say wait for a child process to finish, and then print your numbers as well.
- So what is going to happen is that the main process will wait and the child process will go forward and print the first 5 numbers, finish its execution and get its memory freed and then the main process will continue and print the other 5 numbers.
- If we put wait() here, that is going to be problematic, because we have to wait only in the main process, but now wait() is in both processes.
- Here in the main process, we do have a child process to wait for, but in the child process, we don't have any child process to wait for so we are just going to get stuck there. 
```c
int main(int argc, char *argv[])
{
    int id = fork();
    int n;
    if (id == 0)
    {
        n = 1;
    }
    else
    {
        n = 6;
    wait(NULL); // Wait for the child process to finish
    wait();
    int i;
    for (i = n; i < n + 5; i++)
    {
        printf("%d\n", i);
        fflush(stdout);
    }
    }
    return 0;
}
```

- In order to fix that, we will write it like this:
- What we are saying here is that if we are in the main process, then wait for a child process to finish, and then print the numbers.
- If we are in the child process, then just print the numbers.
- printf("\n"); is not executed once, but twice.

```c
int main(int argc, char *argv[])
{
    int id = fork();
    int n;
    if (id == 0)
    {
        n = 1;
    }
    else
    {
        n = 6;
    }
    if (id != 0)
    {
        wait();
    }
    int i;
    for (i = n; i < n + 5; i++)
    {
        printf("%d\n", i);
        fflush(stdout);
    }
    printf("\n");
    return 0;
}
```

- To avoid that we also use an if condition to check if we are in the main process

```c
int main(int argc, char *argv[])
{
    int id = fork();
    int n;
    if (id == 0)
    {
        n = 1;
    }
    else
    {
        n = 6;
    }
    if (id != 0)
    {
        wait();
    }
    int i;
    for (i = n; i < n + 5; i++)
    {
        printf("%d\n", i);
        fflush(stdout);
    }
    if (id == 0){
        printf("\n");
    }
    return 0;
}
```