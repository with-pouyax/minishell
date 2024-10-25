/* 
- another communication methode between processes that is used all the time is signals
- unlike pipes, where we could send an array, signals are sendind sort of notifications
- a simple integers that says to the other process, what it should do and that is used
to sort of take control of that process.
-for example, if I have a process and I want to pause it, I can send a signal to it
- lets see how we can do this in C.

- in bellows code, our message is printed infinitely, and we want to stop it.
- even if I want to terminate the program using sigint, it will not work, because the
we terminating the parent process, not the child process.

 */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


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
        wait(NULL);
    }
    return 0;
}

/* 
- we can use signals to stop the child process.
- there is a function called kill, that is used to send signals to other processes.
- with kill, we send SIGKILL signal to the pid of the child process and it will stop.
- the child process will umderstand that it should stop and it will stop.
- but how does the child process know that it should stop ?
- the child process is running in a loop, and it is printing "Hello" every 0.5 seconds.
and it is not checking for any signals. so how does it know that it should stop ?
- the answer is that the operating system will stop the process, and it will send a signal
to the process, and the process will stop.
- there is something called event handler in the operating system and it is keep 
listening for signals, and when it receives a signal, it will stop the process.
- the important thing about signals is that they are not following the order of the
code, although we were in the while loop, the signal was sent and the process was stopped.
- signals are sort of outside the code.
- We have also other signals called SIGCHLD, that is sent to the parent process when the
child process is terminated. 
- we also have SIGSTOP, that is used to pause the process, so the process is not
terminated, but it is paused and when it receives SIGCONT signal, it will continue.

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
        sleep(1); // we want to wait for 1 seconds before we send the signal.
        kill(pid, SIGKILL); //first argument is the process id, and the second argument
        //is the signal that we want to send to the process.
        //SIGKILL is a signal that terminates the process. is it a macro ?
        //yes, it is a macro that is defined in signal.h and it is equal to 9.
        wait(NULL);
    }
    return 0;
}