/* 

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int fd[2]; // 
    if (pipe(fd) == -1) // make the pipe
    {
        return 1;
    }
    
    int pid = fork(); // create the child process
    if (pid == -1) {
        return 2;
    }
    
    if (pid == 0) 
    {
        // Child  process
        close(fd[0]);
    
        char str[200]; // array to hold the string
        printf("Input string: "); // ask the user to input a string
        fgets(str, 200, stdin); /* read the string from the user, first argument is 
        the array to store the string, second argument is the maximum number of
        characters to read, and the third argument is the stream to read from, stream
        means where to read from, stdin means read from the standard input */

        str[strlen(str) - 1] = '\0'; /* remove the newline character from the string,
        there is a newline character at the end of the string because when the user
        presses the enter key, a newline character is added to the string and fgets
        reads it as well */
        
        int n = strlen(str) + 1; // get the length of the string + 1 for the null terminator
        if (write(fd[1], &n, sizeof(int)) < 0) /* send the length of the string first
        to the parent process because the parent process needs to know the length of
        the string to read it from the pipe */
        /* we cant instead of &n, use &strlen(str) + 1 because it will happen in 
        cpu so we can't pass the address of the result of an expression */
        {
            return 3;
        }
        printf("Sent n = %d\n", n);
        
        if (write(fd[1], &str, sizeof(char) * n) < 0) /* send the string to the parent
        process */
        {
            return 4;
        }
        
        printf("Sent string\n");
        close(fd[1]);
    }
    else 
    {
        close(fd[1]);
        char str[200]; // array to hold the string
        int n; // variable to hold the length of the string
        
        if (read(fd[0], &n, sizeof(int)) < 0) /* read the length of the string from
        the child process */
        {
            return 5;
        }
        printf("Received n = %d\n", n);
        if (read(fd[0], &str, sizeof(int) * n) < 0) /* read the string from the child
        process */
        {
            return 6;
        }
        printf("Received string %s\n", str);
        
        close(fd[0]);
        wait(NULL);
    }
    
    return 0;
}

// we could also send the string character by character.