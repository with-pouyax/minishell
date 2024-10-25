/*
- we gonna look at two way communication between two processes, a parent and a child
- let's generate a small problem first.
- we have a programm that generates a random number between 0 and 10,
then it send it to other process, what this other process does is to multiply this
number by 4, the result will be sent back to original program, and it prints it.

- now if we run this program it should be fine.
we get :
Wrote number: 3
Received number: 3
Result: 12
Wrote number: 12

- but there is a caveat(هشدار) and the caveat going to becaome very apparent if we comment out
part of code.

  */

 int main()
 {
    int p1[2];
    if(pipe(p1) == -1)
    {
        printf("Error in creating pipe\n");
        return 1;
    }
    int pid = fork();
    if(pid == -1)
    {
        printf("Error in fork\n");
        return 2;
    }
    if(pid == 0)
    {
        int x;
        if (read(p1[0], &x, sizeof(x) == -1))
        {
            printf("Error in reading from pipe\n");
            return 3;
        }
        printf("Received number: %d\n", x);
        x *= 4;
        if (write(p1[1], &x, sizeof(x)) == -1)
        {
            printf("Error in writing to pipe\n");
            return 4;
        }
        printf("Wrote number: %d\n", x);
    }
    else
    {
        srand(time(NULL));
        int y = rand() % 10;
        if(write(p1[1], &y, sizeof(y)) == -1)
        {
            printf("Error in writing to pipe\n");
            return 5;
        }
        printf("Wrote number: %d\n", y);

        if (read(p1[0], &y, sizeof(y)) == -1)
        {
            printf("Error in reading from pipe\n");
            return 6;
        }
        printf("Result: %d\n", y);
    }
    return 0;
}

/* 
- so here I will comment out the part of code.
- now if we run this program, we get:
Received number: 9
and it hangs there.
why is that?
- we probably have 9 from srand() function, because 9 is not divisible by 4.
- When we oppened the pipe at the beginning, we had 4 file descriptors, 
two for parent and two for child, and they are both open for reading and writing.
- so when we write to the pipe, since we have two file descriptors that both parent
and child can read from them, who decides which file descriptor is going to read ?
- in fact, what happens here is that the 9 generated, it is written to the pipe and
immidiately after that, it read in the same process, and the other process has nothing
to read from the pipe, so it hangs at trying to read from the pipe, but there is
nothing to read.
- in the version we did not comment out, because this print is taking some time from
the parent process, the child process has time to read from the pipe

 */

int main()
 {
    int p1[2]; // we initialize our pipe
    if(pipe(p1) == -1)
    {
        printf("Error in creating pipe\n");
        return 1;
    }
    int pid = fork(); // we fork our process
    if(pid == -1)
    {
        printf("Error in fork\n");
        return 2;
    }
    if(pid == 0)
    {
        int x; // we declare a variable x
        if (read(p1[0], &x, sizeof(x) == -1)) // we read from the pipe and store it in x
        {
            printf("Error in reading from pipe\n");
            return 3;
        }
        printf("Received number: %d\n", x); // we print the number we received
        x *= 4; // we multiply the number by 4
        if (write(p1[1], &x, sizeof(x)) == -1) // we write the number to the pipe
        {
            printf("Error in writing to pipe\n");
            return 4;
        }
        printf("Wrote number: %d\n", x); // we print the number we wrote to the pipe
    }
    else
    {
        srand(time(NULL)); // we seed the random number generator
        int y = rand() % 10; // we generate a random number between 0 and 10
        if(write(p1[1], &y, sizeof(y)) == -1) // we write the number to the pipe
        {
            printf("Error in writing to pipe\n");
            return 5;
        }
        //printf("Wrote number: %d\n", y); // we print the number we wrote to the pipe

        if (read(p1[0], &y, sizeof(y)) == -1)
        {
            printf("Error in reading from pipe\n");
            return 6;
        }
        printf("Result: %d\n", y);
        wait(NULL);
    }
    close(p1[0]);
    close(p1[1]);
    return 0;
}

/* 
- what is the solution to this problem?
- we can not have a pipe that both sends data on the same process ?
yes, we cannot do that. and we need two pipes in order to do that.
-so add the second pipe. and lets say that the p1 is the one that goes from child to
parent and p2 is the one that goes from parent to child.
- to set it up right we have to first close some file descriptors, because we will
have to many.

- now if we run this program, it should work fine.
 */

int main()
 {
    int p1[2]; // child => parent
    int p2[2]; // parent => child
    if(pipe(p1) == -1) // we initialize our first pipe
    {
        printf("Error in creating pipe\n");
        return 1;
    }
    if(pipe(p2) == -1) // we initialize our second pipe
    {
        printf("Error in creating pipe\n");
        return 1;
    }
    int pid = fork(); // we fork our process
    if(pid == -1)
    {
        printf("Error in fork\n");
        return 2;
    }
    if(pid == 0) // if we are in the child process
    {
        close(p1[0]); /*  we close the reading end of the first pipe because we use
        p1 only for writing from child to parent, so we do not need it in the child
        process */
        close(p2[1]); /* we close the writing end of the p2 writing end because we
        use p2 only for reading from parent to child, so we do not need it in the child
        process */
        int x; // we declare a variable x
        if (read(p2[0], &x, sizeof(x) == -1)) // we read from the p2 and store it in x
        {
            printf("Error in reading from pipe\n");
            return 3;
        }
        printf("Received number: %d\n", x); // we print the number we received
        x *= 4; // we multiply the number by 4
        if (write(p1[1], &x, sizeof(x)) == -1) // we write the number to the p1
        {
            printf("Error in writing to pipe\n");
            return 4;
        }
        printf("Wrote number: %d\n", x); // we print the number we wrote
        close(p1[1]); // we close the writing end of the p1
        close(p2[0]); // we close the reading end of the p2
    }
    else // if we are in the parent process
    {
        close(p1[1]); /* we close the writing end of the p1 because we use p1 only
        for reading from child to parent, so we do not need it in the parent process */
        close(p2[0]); /* we close the reading end of the p2 because we use p2 only
        for writing from parent to child, so we do not need it in the parent process */
        srand(time(NULL)); // we seed the random number generator
        int y = rand() % 10; // we generate a random number between 0 and 10
        if(write(p2[1], &y, sizeof(y)) == -1) // we write the number to the p2
        {
            printf("Error in writing to pipe\n");
            return 5;
        }
        //printf("Wrote number: %d\n", y);

        if (read(p1[0], &y, sizeof(y)) == -1) // we read the number from the p1
        {
            printf("Error in reading from pipe\n");
            return 6;
        }
        printf("Result: %d\n", y); // we print the result
        close(p1[0]); // we close the reading end of the p1
        close(p2[1]); // we close the writing end of the p2
        wait(NULL); // we wait for the child process to finish
    }
    return 0;
}