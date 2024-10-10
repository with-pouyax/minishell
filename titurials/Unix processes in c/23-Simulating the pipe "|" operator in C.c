/* 
-we already know that with exec family of functions we can run a program but not a
script or shell command.
- so we can't run a command like ping -c 5 google.com | grep rtt using exec functions.

- lets analyze the command ping -c 5 google.com | grep rtt:
- so here we are executing two programs, ping and grep. and other things are arguments
to these programs.
- this two programs are linked by a pipe operator "|".
- it is the same as the pipe we use in C programming, except that it does a few more
things besides just opening a pipe.
- so if we think of this pipe, opening a pipe, that also means that we are actually
opening a pipe taht has two ends and those two ends in this case (our cpommand) are
routed(هدایت شده) in a certain way.
- so we know that when we execute ping -c 5 google.com, it is printing out some
information to the standard output.
- but here it didn't print out anything and it printed out whatever grep found.
- so that mean the output of left program is actually routed through the pipe.
- so ping wrote its output instead of the standard output to this pipe.
- But how grep knows that it should look at those lines?
- that is because also the standard input of grep is routed to the pipe and because of
that since grep knows how to handle standard input, it is not waiting for keyboard 
input, it is actually reading from the pipe.
- now I will draw a diagram to show how this pipe is actually working on other file.
- so as you see in diagram we have a ping process and grep process, that are both made
by main process and we made a pipe between them by main process, which will be flow of
between them from the ping to the grep.

- so first we need to create a pipe.
- then I will create a child process, which will be the ping process.
- in this child process I execute the ping using execlp.
- so as you remember execlp will replace everything inside the executing process,
so this execlp function will will never return anything, is not continuing onwards.
- so we dont need the code of parent process inside an else, because we know everything
after execlp can only execute by parent process because this child process got replaced
by the ping program.
- so after execlp call, I can use waitpid(pid1, NULL, 0).
- we wait here for the child process to finish.

- now we want to reroute the output of our process that started the ping
 */

/* int main(int argc, char *argv[])
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("pipe");
        return 1;
    }

    int pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid1 == 0)
    {
        // child process 1 (ping)
        execlp("ping", "ping", "-c", "5", "google.com", NULL);
    }

    waitpid(pid1, NULL, 0);
    return 0;
} */

/* 
- now we want first to reroute the output of our process that started the ping.
- so before execlp, we want to reroute it.
- remember dup2 function, it is takes in the Fd1 and it duplicates it into fd 2
so fd2 will point to whatever fd1 was pointing to.
- so here we want fd[1] which is the write end of the pipe to be duplicated into the
standard output. so we write dup2(fd[1], STDOUT_FILENO);, which will reroute the
standard output of the child process to the write end of the pipe.
-  before going further to the grep process, we should actually note that we should
close some file descriptors.

- now lets start the grep process.
- so again we fork a child process.
- in child process at the end of it we use execlp to execute the grep program.
- before that we use dup2 to reroute the standard input of the child, like:
dup2(fd[0], STDIN_FILENO);.
- and we close some file descriptors.
- we need of course to wait for the pid2 to finish.


- now if we run this program, it will print out the output of the ping program, 
but there is an issue because our program hangs.
why it hangs?
 */

// #include <stdio.h>
// #include <unistd.h>
// #include <sys/wait.h>


// int main(int argc, char *argv[])
// {
//     int fd[2];
//     if (pipe(fd) == -1)
//     {
//         perror("pipe");
//         return 1;
//     }

//     int pid1 = fork();
//     if (pid1 == -1)
//     {
//         perror("fork");
//         return 1;
//     }

//     if (pid1 == 0)
//     {
//         // child process 1 (ping)
//         dup2(fd[1], STDOUT_FILENO); /* what it does is it duplicates fd[1], so now we will
//         have two file descriptors that point to the. one is fd[1] and the other one is
//         STDOUT_FILENO. (if you got cunfuse review the dup2 lesson) */
//         close(fd[0]); // close the read end of the pipe because we dont use it.
//         close(fd[1]); /* close the write end of the pipe because fd[1] is remained
//         open although we have duplicated it, because dup2 does only move the file
//         descriptor, but it actually makes another one. */
//         execlp("ping", "ping", "-c", "5", "google.com", NULL);
//     }

//     int pid2 = fork();
//     if (pid2 == -1)
//     {
//         perror("fork");
//         return 1;
//     }
//     if (pid2 == 0)
//     {
//         // child process 2 (grep)
//         dup2(fd[0], STDIN_FILENO); // reroute the standard input of the child process to the read end of the pipe.
//         close(fd[0]); // close the read end of the pipe because we dont use it.
//         close(fd[1]); // close the write end of the pipe because we dont use it.
//         execlp("grep", "grep", "rtt", NULL);
//     }

//     waitpid(pid1, NULL, 0);
//     waitpid(pid2, NULL, 0);
//     return 0;
// }

/* 
-the issue is that the grep keeps up reading from whatever pipe we have, whatever file
descriptor we gave it.
- it keeps reading the input from the standard input, which is the read end of the pipe.
but since it is a pipe, when does it know, when to stop reading? it is like okay I can
wait here and keep reading, but untill when?
- whell that actually happens when all the writers to the pipe are closed.
- so we actually have multiple writers open on the pipe.
- so as you remeber a pipe a pipes or really any file descriptor, get inherited when
we call fork.
- so each of the 3 processes (main process, ping process, grep process) have access to
this pipe that we created.
- we have closed them on grep process, on the ping process, though we have duplicated
the ping process's the file descriptor, (something nice about linux processes is that
they close theire stdout once they are done.
- so once the ping finshes its execution, it does automatically close fd[1], then what
writer is open ? 
- Well thats actually the writer in the main process.
- the main process has still fd[0]  and fd[1] open.
- that is a pretty big issue because since the main process has still, the write end
opened for the pipe, the grep process still waits on that pipe to get input, it is 
only until we close that write end, that the grep process will know that it has reached
the end of the input.
- so we have to close both ends of our pipe.

 */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[])
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("pipe");
        return 1;
    }

    int pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid1 == 0)
    {
        // child process 1 (ping)
        dup2(fd[1], STDOUT_FILENO); /* what it does is it duplicates fd[1], so now we will
        have two file descriptors that point to the. one is fd[1] and the other one is
        STDOUT_FILENO. (if you got cunfuse review the dup2 lesson) */
        close(fd[0]); // close the read end of the pipe because we dont use it.
        close(fd[1]); /* close the write end of the pipe because fd[1] is remained
        open although we have duplicated it, because dup2 does only move the file
        descriptor, but it actually makes another one. */
        execlp("ping", "ping", "-c", "5", "google.com", NULL);
    }

    int pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork");
        return 1;
    }
    if (pid2 == 0)
    {
        // child process 2 (grep)
        dup2(fd[0], STDIN_FILENO); // reroute the standard input of the child process to the read end of the pipe.
        close(fd[0]); // close the read end of the pipe because we dont use it.
        close(fd[1]); // close the write end of the pipe because we dont use it.
        execlp("grep", "grep", "rtt", NULL);
    }

    close(fd[0]); 
    close(fd[1]);
    
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;
}