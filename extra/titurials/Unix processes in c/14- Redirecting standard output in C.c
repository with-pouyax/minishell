/* 
- here we continue from the code from 12-
- right now when I run the programm, I get the output on the terminal, but I don't want
to get ping messages, I just want to know if we were able to ping the server or not.
- How can I tell my ping program to stop printing on the terminal ? 
- In linux, we can actually do that.
- lets say we want to output all the data into another file, How do we do that ?
- firs we have to create and open a file, we can do it easily using open() function.
- after that we have the file, but how could we say that we want the output to go to 
this file instead of the terminal ?
- so first we have to learn how the file descriptors and file situations is in linux.
- first imagine we are on child process.
- what are file descriptors ?
- the open() function returns a file descriptor.
- mkfifo() function also returns a file descriptor.
- when we make a pipe, we get two file descriptors.
- what file descriptors is, is just a number that is unique across the process, so
two processes can have the same file descriptor with number 3, but the same process
can't have two file descriptors with the same number. and these really represent a
key or a handle to an input or output resource. 
- that could be a file, that could be the terminal input, the terminal output, or
stdin, stdout, stderr, or a pipe and so on.
- there are just things that you can read or write to and whenever you open a process
automatically linux starts and open certain file descriptors for you.
- so automatically the file descriptor with id 0 is actually linked and oppend to handle
stdin, that means that if you would call read(0), that would be similar to call
scanf(), although no formating, just basicallly reads the bytes you're getting.
- the file descriptor with id 1 is going to be stdout, so if you call write(1, "hello", 5),
it will output on the terminal. 
- and the file descriptor with id 2 is stderr. so this where all the error messages go to
- so these 3 file descriptors are opened by default before you get even to first line
in the main function, they are already opened.
- and whenever we open a file, this file gonna have a file descriptor, which is
pointing to that file.
- what we want is to have output to this ping_results.txt file.
- so we want actually to replace file descriptor 1 with the file descriptor of the
file we just opened.
- How do we do that ?
- in linux, there is a function called dup() function, which takes single argument,
which is the file descriptor.
- so if we give it the file descriptor of the file we just opened, it will return
another file descriptor for our ping_results.txt file, and this new file descriptor
also points to the same file.
- so we gonna have two file descriptors pointing to the same file.
- there is also another function called dup2(), which takes two arguments, 
the first argument is the file descriptor we want to duplicate, and the second argument
is the value that we want the new file descriptor to have.
- so if we call dup2(file, 1);, it will replace the file descriptor 1 with the
file descriptor of the file we just opened.
- but what this is going to do is instad of creating another value in pnig_results.txt,
it gonna look at 1 and first it will close the file descriptor 1(like delete it), and
it gonna open it again to our ping_results.txt file.
- so we gonna have :
    0 -> stdin
    1 -> ping_results.txt
    2 -> stderr
    3 -> ping_results.txt
- it will overwrite everything, the code, the memory but it doesn't change the
process id nor the file descriptor that we have already opened. that means that
when we change the stdout to ping_results.txt before we call the ping program,
it is going to change it inside the ping program, when the ping program prinfs
something, it will go to the ping_results.txt file, instead stdout.
- instead of dup2(file, 1);, we can dup2(file, STDOUT_FILENO);, stdout_fileno is
a macro that is defined in the unistd.h file, and it is equal to 1.
- it is better to use the macro, because it is more readable and in some machines
it could be different.
- Before we finish, the only issue is remember we have two file descriptors that
are open to ping_results.txt file, that's not really a problem, but we're going to
use one of them, so we should close the one that we're not going to use.
so we just call close(file);, which is our original file descriptor for the file.


- if we lunch this now, we will see something interesting:

The fd to ping_results: 3
success
post processing goes here

- so we got our first message, but we didn't get our second message, which is
The duplicated fd: 1
- why is that ?
- because we have already swapped the stdout, so after first printf, the second
printf is going to write to the ping_results.txt file, not to the terminal.
- in fact we don't need this.
- so if we remove the second printf, and run the program, we will not get anythig
from the ping on the terminal.
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
        int file = open("ping_results.txt", O_WRONLY | O_CREAT, 0777); // first argument is the file name, second argument is
        // the flags, O_WRONLY means we want to write to the file, O_CREAT means we want to create the file if it doesn't exist
        // 0777 is the permission. the 0 of 0777 means that we are using octal number.
        if (file == -1)
        {
            printf("Could not open the file\n");
            return 1;
        }
        printf("The fd to ping_results: %d\n", file);
        int file2 = dup2(file, STDOUT_FILENO);
        printf("The duplicated fd: %d\n", file2);
        close(file);

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