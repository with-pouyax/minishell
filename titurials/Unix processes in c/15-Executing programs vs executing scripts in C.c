/* 
- we want to take a look at a missunderstanding that we could have, regarding the exec
functions.
- so if we run the program, we will get the ping results and a success message.
- now suppose that all I want from this ping program is to print, is not all the pings
themselfs, not the statistics, all I want is round trip time.(this is wha rtt stands
for). (it is part of what the ping program prints).
-suppose that we know about grep, grep in linux let us sort of filter out the output
of a program.
- for example what I can do inside Terminal is to write:
    ping -c 3 google.com => this the same command that we are running in the program
- but what I want is just to filter it out, so I can write:
    ping -c 3 google.com | grep "rtt"
- what this does is that it takes the output of the ping program and filters it out
and only prints the lines that contain the string "rtt".
- How can we do this in our program ?
- you might think that we can do this by exec functions, by adding more arguments to
the exec functions, like this:
int err = execlp("ping" , "ping", "-c", "1", "google.com", "|", "grep", "\"rtt\"", NULL); 
- if I try to run the program, I will get an error message that says:
    ping: "rtt": Name or service not known
    Failed
- why did this happen ?
- here is the missunderstanding that the exec expects bash script or command line
scripts.
- in fact that is not true, what it does is that it executes an executable fil, not
a command line.
-so in our scenario, it finds the ping executable, and executes it without going through
bash interpreter.
-so what happened here is c directly talking with executable, and in our case with the
bash script, the bash script execute that executable, but this pipe "|" operator is
specific to bash, so it is not going to work.
- but in bash, it works like it executes the ping program, and then it executes the
grep program, and then it pipes the output of the ping program to the grep program,
because the pipe operator is specific to bash and not to the ping program. and pipe
operator is not an executable file. that is why we can't run | alone in the terminal.
- so remember that exec, executes an executable file, not a command line script.
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
        int err = execlp("ping" , "ping", "-c", "3", "google.com", NULL);
        if (err == -1)
        {
            printf("Could not find the program to execute\n");
            return 1;
        }
    }
    else
    {
        int wstatus;
        wait(&wstatus);
        if (WIFEXITED(wstatus))
        {
            int status_code = WEXITSTATUS(wstatus);
            if (status_code == 0)
            {
                printf("Success\n");
            }
            else
            {
                printf("Failed\n");
            }
        }
        printf("some post processing goes here\n");
    }
    return 0;
}