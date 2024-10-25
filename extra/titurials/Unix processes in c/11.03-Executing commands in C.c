/*
- in these pre lessons we learned, there is a caveat with exec function
that the code after exec function will not be executed
- not only your program would stop executing, our process would be replaced by the new process.
- let's first call a simple ping program.
- because we are in linux and we dont want it to continue forever, we can use the -c flag to ping only once.
- it is working fine.
*/

int main(int argc, char *argv[])
{
    execlp("ping" , "ping", "-c", "3", "google.com", NULL);
    return 0;
}

/* 
- now let's print success message after the ping program is executed.
-I will get the ping 3 times but no success message.
- now the we learned processess we can do something.
- we can make a new process, so we cann call execlp and also execute something after it.
- so we first fork, after this we gonna have 2 processes. so we have a parent and a child process.
- it is important which one we choose to execute the execlp function, because.
- remember with processes, it is important to wait for the child process to finish. but if we exec on the pannel process. well the parent
process will be replaced by the new process and it can't no longer wait for the child process. that may do some post processing.
- so we can not call execlp on the parent process and expect it work properly. the child process will be zombie process at the end.
- so because of that we should call child process inside the child process and inside the parent process what we have to do is to wait for
that process to finish.
 */

int main(int argc, char *argv[])
{
    int pid = fork();
    if (pid == -1)
    {
        return 1;
    }
    if (pid == 0)
    {
        execlp("ping" , "ping", "-c", "3", "google.com", NULL);
    }
    else
    {
        wait(NULL);
        printf("Success\n");
        printf("post processing goes here\n");
    }


    execlp("ping" , "ping", "-c", "3", "google.com", NULL);
    printf("Success\n");
    return 0;
}

/* 
- we could also make a Fifo doing this.
- here we make a Fifo using linux command, instead of using the C code.
 */

int main(int argc, char *argv[])
{
    int pid = fork();
    if (pid == -1)
    {
        return 1;
    }
    if (pid == 0)
    {
        execlp("mkfifo" , "mkfifo", "sum", NULL);}
    else
    {
        wait(NULL);
        printf("Success\n");
        printf("post processing goes here\n");
    }


    execlp("ping" , "ping", "-c", "3", "google.com", NULL);
    printf("Success\n");
    return 0;
}

/* 
- something really important to take away from this is that, the child process that we have is not a magic properties in which it keeps
executing, the child process got replaced by this ping process. so because of that replacement, we no longer have a child process, that
we can program our own,
 */