
# Forking and Process IDs in C

This repository demonstrates how forking in C works and how process IDs are managed across parent and child processes.

## Introduction

In Unix-based systems, such as Linux, the `fork()` system call is used to create new processes. A new process created by `fork()` is a copy of the parent process, and both processes will run concurrently. The main difference is the process ID, which is unique to every process.

### Key Notes:
1. `fork()` creates a new process, which is an exact copy of the parent process.
2. The parent and child processes continue executing the same code after the `fork()`.
3. The `fork()` function returns:
   - `0` in the child process.
   - The child's process ID (PID) in the parent process.
   - `-1` if the process creation failed.

## Code Walkthrough

### Simple Fork Example

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    fork();  // Creates a new process
    printf("Hello world\n");
    return 0;
}
```

In this example, after calling `fork()`, both the parent and child processes print "Hello world". Hence, the output will be printed twice.

### Process IDs and Conditional Execution

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

This code prints the process ID for both the child and parent processes. The child process will have a PID of 0, while the parent process will display the actual PID of the child.

### Multiple Forks

If you call `fork()` multiple times, the number of processes increases exponentially. For example:

```c
int main(int argc, char *argv[])
{
    fork();
    fork();
    printf("Hello world\n");
    return 0;
}
```

Here, "Hello world" will be printed 4 times because 2 forks create 4 processes in total. The formula to calculate the number of processes is `2^n`, where `n` is the number of forks.

### Creating a Specific Number of Processes

To create exactly 3 processes, we can use an `if` condition to control the forking process:

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

In this case, "Hello world" will be printed 3 times.

## Compilation and Execution

To compile the program:

```bash
gcc -o fork_example fork_example.c
```

To run the program:

```bash
./fork_example
```

## Conclusion

Understanding how the `fork()` function works is essential in managing multiple processes in a Unix-like operating system. By using process IDs and conditional statements, you can control which process executes which part of your code.
