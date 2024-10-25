/* 
- if we have more than one fork() in a program, how can we figure out which process is which?
- if I put wait(NULL)  here, it is problematic because the wait() function
only waits for one child process to finish. so if one of the child processes
finishes it doesn't mean that the other child process has finished.
 */

int main(int argc, char *argv[])
{
    int id1 = fork();
    int id2 = fork();
    if (id1 == 0) // that means we are a child of the parent process
    {
        if (id2 == 0) // that means we are a child of the child process
        {
            printf("we are process y\n");
        }
        else // we are still a parent process because ID1 is 0 but we are not
             //a process that doesn't have any child, we actually have a child process
        {
            printf("we are process x\n");
        }
    }
    else // if ID1 is not 0
    {
        if (id2 == 0) // if ID1 is not 0 and ID2 is 0, that means we are a child of the
                        // parent process
        {
            printf("we are process z\n");
        }
        else // if ID1 is not 0 and ID2 is not 0, that means we are the parent process
        {
            printf("we are the parent process\n");
        }
    }
            
            
    wait(NULL);
    return 0;
}

/* 

 */

int main(int argc, char *argv[])
{
    int id1 = fork();
    int id2 = fork();
    if (id1 == 0)
    {
        if (id2 == 0)
        {
            printf("we are process y\n");
        }
        else
        {
            printf("we are process x\n");
        }
    }
    else
    {
        if (id2 == 0)
        {
            printf("we are process z\n");
        }
        else
        {
            printf("we are the parent process\n");
        }
    }
            
            
    while (wait(NULL) != -1 || errno != ECHILD); // now we wait until all child processes
    // have finished executing, we receive -1 when there are no child processes to wait for
    //and while errno is not ECHILD, that means that there are still child processes to wait
    // for. errno is a global variable that is set by the system when an error occurs.
    // ECHILD is a macro that is defined in the errno.h header file, and errno will be
    // set to ECHILD when there are no child processes to wait for.
    return 0;
}

/* 
-or we can do something in the while loop, and I will get 3 messages because
I have 3 child processes
 */

int main(int argc, char *argv[])
{
    int id1 = fork();
    int id2 = fork();
    if (id1 == 0)
    {
        if (id2 == 0)
        {
            printf("we are process y\n");
        }
        else
        {
            printf("we are process x\n");
        }
    }
    else
    {
        if (id2 == 0)
        {
            printf("we are process z\n");
        }
        else
        {
            printf("we are the parent process\n");
        }
    }
            
            
    while (wait(NULL) != -1 || errno != ECHILD)
    {
        printf("waiting for child process to finish\n"); // now we have to print this
        // message 3 times, because we have 3 child processes  
    }
    return 0;
}
