# Forking and Process IDs in C

- To use the `fork()` function in C, we need to include `unistd.h`. Note that `unistd.h` is a Linux-specific library, so on Windows, we use other functions to create processes.
- If we add a `fork()` before `printf`, "Hello world" will be printed twice.
- What `fork()` does is create a new process, and the new process is a copy of the parent process. The new process will have the same code and data as the parent process, but it will have a different process ID. We can save the process ID of the new process in a variable.

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    fork();
    printf("Hello world\n");
    return 0;
}
```

- We can save the process ID of the new process in a variable.
- Now we will receive, for example:
    Hello world from id: 0
    Hello world from id: 4222
- Why?
- Everything before the line of `fork()` is going to be executed once.
- From the `fork()` line onwards, everything is going to be executed twice, once per process.
- So we're going to have two separate IDs being returned from `fork()`. One will be the main process with ID 4222, and the other will be the child process with ID 0.
- The ID of the child process is always 0, and it returns -1 if the `fork()` function fails.
- Every process in Linux, Windows, and other operating systems has a process ID.
- We can add a condition to show whether we are in the child process.

```c
int main(int argc, char *argv[])
{
    int id = fork();
    printf("Hello world from id: %d\n", id);
    if (id == 0)
    {
        printf("I am the child process\n");
    }
    else
    {
        printf("I am the main process\n");
    }
    return 0;
}
```

- What would happen if I called `fork()` twice?
- How many times will "Hello world" be printed?
- 4 times.
- Why?
- Because the first `fork()` will create a child process,
- and the second `fork()` will create a child process for the child process, so we will have 4 processes in total.
- The way we can calculate the number of processes is 2^n,
- so if we call `fork()` 3 times, we will have 8 processes.
- But how can we create only 3 processes?

```c
int main(int argc, char *argv[])
{
    fork();
    fork();

    printf("Hello world\n");
    return 0;
}
```

- By using an `if` condition to create only 3 processes. We are saying that if we are not in the child process, then create a new process.

```c
int main(int argc, char *argv[])
{
    int id = fork();
    if (id != 0)
        fork();

    printf("Hello world\n");
    return 0;
}
```