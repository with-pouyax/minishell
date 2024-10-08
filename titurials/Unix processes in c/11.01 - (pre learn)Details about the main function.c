/* 
- the return type of main must always be int, because the operating system is
going to look at the return value of the main function to see if the program
executed successfully or not.
- why we can compile main, without any return value?
- the ide is going to add a return 0 at the end of the main function automatically,
when there is no return value.
- other than argc and argv, we can have a third argument in the main function
called envp.
- envp is an array of char pointers. it can be any name, but it is usually called envp.
- envp stores the environment variables.
-envp (Environment Pointer): It's an additional argument that can be passed to the
main function, typically used alongside argc (argument count) and argv (argument values).
- It is an array of character pointers (char *envp[]), where each element points to
a string that represents an environment variable in the form of "KEY=VALUE".
For example, one element might look like "PATH=/usr/bin:/bin".

-Environment Variables: These are dynamic values used by the operating system and
programs to store information that can affect the behavior of processes.
0They provide information like:
-PATH: Specifies the directories where executable programs are located.
-HOME: Indicates the current user's home directory.
-USER: Holds the name of the current user.
-Programs use these environment variables to adapt to different system settings or
configurations without hardcoding paths and values.
-For example, a program might check the PATH environment variable to find the location
of executables it needs to run. 
-When the program starts, the OS passes these variables to the program through envp.

-the weird thing about envp is the way we iterate through it.
-it is different from argv because argv has a counter, but this guy is an array of
strings that is terminated by a NULL pointer.

- if I run this program, I get whole bunch of environment variables. (so many)

 */

#include <stdio.h>
int main(int argc, char *argv[], char *envp[])
{
    for (int i = 0; envp[i] != NULL; i++)
    {
        printf("envp[%d] = %s\n", i, envp[i]);
    }
    return 0;
}