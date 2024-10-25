/* 
- How to control the execution of a process using
SIGCONT and SIGSTOP signals in C.
- In the below code, I want to control, actually
weather or the child process executes from the parent using signals.
- First to make sure that the proccess finish its execution somehow, we gonna call
kill(pid, SIGKILL) right before wait(NULL) in the parent process.and before that
we gonna add where we use sigcont and sigstop signals.
- but first we need to add a sleep(1) in the beginning of the parent process to make
sure that the child process starts executing before we send the signals.
- now if we run the code, we will see that the child process will start executing
printing "Hello" every 0.5 seconds, and then after 1 second the parent process will
send a SIGSTOP signal to the child process, and the child process will stop executing.
- but what I want is to create a program that what it does is it prompts me to give 
number of seconds, and from then on the child process will start executing for exactly
five seconds, and after those five seconds have elapsed (گذشت), it stops executing.
- it doesn't necessarily kill or terminate it's process, it just kind of goin to stop
execution and I get another prompt to give another number of seconds, and have it
execute for another set of seconds, how do we do that?
-First how we can stop the process ?
- If I send a SIGSTOP at the beginning of parent process, we will see something interesting:

we will see that nothing printed on the screen, because what happened is child
is created and it tries to execute, but then the parent process comes and sends the 
sigsop command, then the child process stops then we wait a second, then we kill the
child process.

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
        while (1)
        {
            printf("Hello\n");
            usleep(500000);
        }

    }
    else
    {
        kill(pid, SIGSTOP);
        sleep(1);


        kill(pid, SIGKILL);
        wait(NULL);
    }

    return 0;
}

/* 
- how about we stop it, we wait for a second and then we let it continue it's execution
- now if we run the code, we will get:
- for 1 second nothing will be printed on the screen, then after 1 it will print "Hello"
every 0.5 seconds, and then after 1 second it will stop executing.

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
        while (1)
        {
            printf("Hello\n");
            usleep(500000);
        }

    }
    else
    {
        kill(pid, SIGSTOP);
        sleep(1);
        kill(pid, SIGCONT);
        sleep(1);

        kill(pid, SIGKILL);
        wait(NULL);
    }

    return 0;
}

/* 
- now lets try to put it in the practice.
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
        while (1)
        {
            printf("Hello\n");
            usleep(500000);
        }

    }
    else
    {
        kill(pid, SIGSTOP); /* here we first stop the child process from doing anything
        before we prompt the user to give the number of second, because we don't want 
        the child process to start executing before we prompt the user to give the number
        of seconds. */ 

        int t;
        do {
            printf("Time in seconds for execution: ");
            scanf("%d", &t);

            if (t > 0) // we check for this because if we put -1 into sleep, it will
            // equal to max int value.
            {
                kill(pid, SIGCONT); // here we let the child process to continue it's
                // execution because we have the number of seconds that we want it to execute
                sleep(t); // here we let the parent process to sleep for t seconds
                // in this t seconds the child process will be executing and printing.
                kill(pid, SIGSTOP); // after t seconds have elapsed, we stop the child
                // process because we don't want it to continue executing.


        }
        while (t > 0);

        kill(pid, SIGSTOP);
        sleep(1);
        kill(pid, SIGCONT);
        sleep(1);

        kill(pid, SIGKILL);
        wait(NULL);
    }

    return 0;
}

/* 
-the key thing here is the while loop, because this while loop is a placehoder for
something more high level, lets say it is listener for some sort of request from a web
server, or something like that and it contiues listening for data, and one point
I just want to pause the execution, so it no longer listens for data, and I can change
a couple of things and start the execution again programmatically.
 */