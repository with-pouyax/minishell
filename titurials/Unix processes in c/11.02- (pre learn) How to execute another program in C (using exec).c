/* 
- How to execute another program in my C program?
- we have a command called ping, which is used to check the network connection, and we
want to execute this command in our C program.
- to do that, we have to use a set of functions called exec family of functions and
those are standard functions in linux
- to start we have to call a function called execl.
- what this function does is that, it basically taking a file name to an executable
and then executes that file.
- the first argument is the path to the executable,
- the second argument is again the path to the executable, but it is the name of the
executable, and it is the name that is going to be displayed in the process list.
- the third argument is the first argument that is going to be passed to the executable
- the fourth argument is the second argument that is going to be passed to the executable
- and so on.
- if we want to pass a NULL argument, we have to pass NULL as the last argument.



 */

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    execl("/bin/ping", "/bin/ping", "google.com", NULL);
    return 0;
}
/* 
- now what if we want to print something after this ?
- now if we run this program, we will not get the "End of program" message.
- why is that?
- because this execl and all the other functions in the exec family, what they actually
do is.
imagine I have execl process and ping process, the execl function doesn't just sort of
launch a new process and wait until it finishes and comes back to execute the rest of
process from our main function.
- it actually takes the new ping process and and put it on top of it.
- basically it overrwiting everything that we had before, not only just memory, but the
actual execution.
after execl is called, we no longer are in our program, we executing this ping program
and that's it.
- once the ping program is done executing, we are done. nothing can happen after that,
unless there is an issue with the ping program and it cant work properly.
 */

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    execl("/bin/ping", "/bin/ping", "google.com", NULL);
    printf("End of program\n");
    return 0;
}

/* 
- instead of execl, we can use another function called execlp.
- that p there stands for path, and it actually use path variable, the path variable
is the one that loaded automatically in the command prompt.
- when you type a command in the command prompt, the command prompt automatically
going to look at all the directories that we added there and of course this ping program
is in there
- here l stands for list of arguments, so here we executed the ping program with a list
of arguments.
 */

int main(int argc, char *argv[])
{
    execl("ping", "ping", "google.com", NULL);
    printf("End of program\n");
    return 0;
}

/* 
- we also have another function called execv.
- v stands for vector, and it takes an array of arguments.
- we can also have execvp, which is the same as execv, but it uses the path variable.
- so here instead of passing the arguments one by one, we can pass them as an array.
 */

int main(int argc, char *argv[])
{
    char *arr[] = {
        "ping",
        "google.com",
        NULL};
    execv("ping", arr);
}

/* 
- we also have another function called execve.
- e stands for environment, and it takes an array of environment variables.
- so we can have also execvpe.
 */

int main(int argc, char *argv[])
{
    char *arr[] = {
        "ping",
        "google.com",
        NULL
    };
    execv("ping", arr);

    char *env[] = {
        "TEST=environmnet variable",
        NULL
    };
}