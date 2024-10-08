/* 
- contine to last example:
- How to check if this program that we launched was successful or not?
- there are two types of errors here:
1- if the actual ping programm does not exist or found.
2- if the actual ping is found and executed but it gives out an error.

- How can we treat each o fhe error types here ?

- the first one is easy, remember what execlp does, is basically replacing the current process and memory with the new process and memory only
if it is successful. if not, it will not replace the process and memory and it continues to execute the rest of the program.
- so in fact, it will return an integer that represents whether it was successful or not. so we can store this value in a variable
- But what if the ping is wrongly spelled, like "pings" ?
- so if I lunch this now what will happen ?
-so it will print "Could not find the program to execute" because the program was not found.
- But what if we write wrong domain name, like "google.con" ?
now we gonna get:
ping: google.con: Name or service not known
success
- the problem is we printed success message, even if the ping program was not successful.
- How can we fix this ? How can we find out if the ping program was successful or not ?
- that answer lies in the wait function.
- remember every time we called wait with NULL. 
-  in fact this parameter tells us what happened with the child process.


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
        wait(NULL);
        printf("Success\n");
        printf("post processing goes here\n");
    }


    execlp("ping" , "ping", "-c", "3", "google.com", NULL);
    printf("Success\n");
    return 0;
}

/* 
- the argument of wait is an int pointer, and it will store the exit status of the child process.
- so I can store the exit status of the child process in a variable. like int wstatus and pass it to the wait function as an argument
like this wait(&wstatus).
- now after this has executed, something will be populated into here that will tell us what happened with the child process.
- but we have to make use of some macros to understand what happened.
- so there are two checks that we have to do with this wstatus variable.
- after it finished, its execution, we have to test if the code, if the program just simply exited normally, nothing went wrong.no external
signal was sent to it.
- to check that we use a if statement like below:
if (WIFEXITED(wstatus)), WIFEXITED is a macro that checks if the program exited normally or not, and we pass the wstatus variable to it, because
it is the one that holds the exit status of the child process. and it is not a function but it receives a parameter.
- inside this if statement, we can take the status code that the ping program exited with like this:
int status_code = WEXITSTATUS(wstatus);
- what this status code is ? that is the return value inside the main or the value that it exits with using the exit code
- so now if I run this program with google.con, I will get:
ping: google.con: Name or service not known
Failed
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
        printf("Success\n");
        printf("post processing goes here\n");
    }


    execlp("ping" , "ping", "-c", "3", "google.com", NULL);
    printf("Success\n");
    return 0;
}