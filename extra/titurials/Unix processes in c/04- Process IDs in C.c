/* 
what are process IDs?
- they are just identification numbers for processes.
- so here using the getpid() function, we can get the process ID of the current process.
- so we will get two process IDs, one for the main process and one for the child process.
- we gonna receive 3727 and 3722.
- first one is the main process, and the second one is the child process.

 */

int main(int argc, char *argv[])
{
    int id = fork();
    printf("id\n", getpid());
    return 0;
}

/* 
-another thing we can do is to get the process ID of the parent process.
- to do that, we can use the getppid() function.
- it is similar to getpid() function, but it returns the process ID of the parent process.
- now if I run this code, I will get :
current process ID: 3784, parent process ID: 3778
current process ID: 3778, parent process ID: 3772
- so every process in the system has a parent process ID, except the process with ID 0,
that is the main process that launches everything.
 */

int main(int argc, char *argv[])
{
    int id = fork();
    printf("current process ID: %d, parent process ID: %d\n", getpid(), getppid());
    return 0;
}
/* 
- so as you remember, we can wait for one process to wait for another process to finish.
- we actually in parent process, we have waited for the child process to finish.
- because always the child process should terminate before the parent process.
and if it happens the other way around, something interesting is happening.
- let's do that.
- so we can say if the process ID is 0, then sleep for 1 second.
- so what happens is that both processes will start executing at the same time,
but since the child process is sleeping for 1 second, the parent process will be
terminated before the child process.
- so it gonna get to printing statements, and it will print the process ID of the parent ID,
 so the question is, what is the parent process ID of that process ?
 so if I execute this code, I will get:
current process ID: 3843, parent process ID: 3837
[1]+  Done                    "/usr/bin/gdb" --interpreter=mi --nx
press any key to continue...current process ID: 3848, parent process ID: 1077

-so here the parent ID was 3843, but it actually terminated, and because it terminated,
with having still child process running, well a new parent process has been assigned to
the child process. which is 1077.
-  so normally we have to get the same parent process ID, but in this case we got a
different parent process ID.
- it considered as a zombie process because if you try to kill it with sigkill command,
it will not actually free the memory. which is a memory leak.

 */
int main(int argc, char *argv[])
{
    int id = fork();
    if (id == 0)
    {
        sleep(1);
    }
    
    printf("current process ID: %d, parent process ID: %d\n", getpid(), getppid());
    return 0;
}

/* 
- so to avoid that, we can use the wait() function.
- we pass NULL as an argument to the wait() function.
- even we could wait withoud specifying any if condition.
 */

int main(int argc, char *argv[])
{
    int id = fork();
    if (id == 0)
    {
        sleep(1);
    }
    
    printf("current process ID: %d, parent process ID: %d\n", getpid(), getppid());
    
    if (id != 0)
    {
        wait(NULL);
    }
    
    return 0;
}

/* 
- even we could wait withoud checking if id is a parent
 */

int main(int argc, char *argv[])
{
    int id = fork();
    if (id == 0)
    {
        sleep(1);
    }
    
    printf("current process ID: %d, parent process ID: %d\n", getpid(), getppid());
    
    wait(NULL);
    
    return 0;
}
/* 
- we can wait because, what its gonna do is, it will check if there is any child process
to wait for. and if it isn't, it will just gonna return -1.
 */

int main(int argc, char *argv[])
{
    int id = fork();
    if (id == 0)
    {
        sleep(1);
    }
    
    printf("current process ID: %d, parent process ID: %d\n", getpid(), getppid());
    
    int result = wait(NULL);

    if (result == -1)
    {
        printf("no child process to wait for\n");
    }
    else
    {
        printf("%d finished execution\n", result);
    }
        
    
    return 0;
}