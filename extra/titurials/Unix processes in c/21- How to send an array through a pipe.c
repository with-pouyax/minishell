/* 
- Here we want to solve a problem :

- first I want to have 2 processes
- Child process should generate random numbers and send them to parent.
- parent will receive the numbers and sum them up and print the result.

- we wanted to see the way we actually send an array through a pipe.
- so we always have to sent first the number of elements and then the array itself.

 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char* argv[]) 
{
    int fd[2]; // array to hold the file descriptors
    if (pipe(fd) == -1) // create the pipe 
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
        close(fd[0]); // close the read end of the pipe because we are going to use the write end
        
        int n, i;
        int arr[10]; // array to hold the random numbers
        
        srand(time(NULL));
        n = rand() % 10 + 1; // generate a random number between 1 and 10 and store it in n
        
        printf("Generated: "); 
        for (i = 0; i < n; i++) // generate n random numbers and store them in the array
        {
            arr[i] = rand() % 11;
            printf("%d ", arr[i]); // print the generated number
        }
        printf("\n");
        
        if (write(fd[1], &n, sizeof(int)) < 0) // write the number of elements first to the pipe
        {
            return 3;
        }
        printf("Sent n = %d\n", n); // print the number of elements
        
        if (write(fd[1], &arr, sizeof(int) * n) < 0) // write the array to the pipe
        {
            return 4;
        }
        
        printf("Sent array\n"); 
        close(fd[1]); // close the write end of the pipe
    } 
    else 
    { // Parent process

        close(fd[1]); // close the write end of the pipe because we are going to use the read end
        int arr[10]; // array to hold the received numbers
        int n, i, sum = 0; 
        
        if (read(fd[0], &n, sizeof(int)) < 0) // read the number of elements from the pipe
        {
            return 5;
        }
        printf("Received n = %d\n", n); // print the number of elements
        if (read(fd[0], &arr, sizeof(int) * n) < 0) // read the whole array from the pipe
        {
            return 6;
        }
        printf("Received array\n"); // print the received array
        
        close(fd[0]); // close the read end of the pipe
        for (i = 0; i < n; i++) // sum up the numbers in the array
        {
            sum += arr[i];
        }
        printf("Result is %d\n", sum); // print the result
        wait(NULL); // wait for the child process to finish
    }
    
    return 0;
}


/* 
- bellows is exactly the same code but with a small change in the way we send the array
- instead we send and receive ellments one by one.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char* argv[]) 
{
    int fd[2]; // array to hold the file descriptors
    if (pipe(fd) == -1) // create the pipe 
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
        close(fd[0]); // close the read end of the pipe because we are going to use the write end
        
        int n, i;
        int arr[10]; // array to hold the random numbers
        
        srand(time(NULL));
        n = rand() % 10 + 1; // generate a random number between 1 and 10 and store it in n
        
        printf("Generated: "); 
        for (i = 0; i < n; i++) // generate n random numbers and store them in the array
        {
            arr[i] = rand() % 11;
            printf("%d ", arr[i]); // print the generated number
        }
        printf("\n");
        
        if (write(fd[1], &n, sizeof(int)) < 0) // write the number of elements first to the pipe
        {
            return 3;
        }
        printf("Sent n = %d\n", n); // print the number of elements
        
        for (int i = 0; i < n; i++)
        {
            if (write(fd[1], &arr[i], sizeof(int)) < 0) // write one element to the pipe
            {
                return 4; // Return error if the write operation fails
            }
        }
        
        printf("Sent array\n"); 
        close(fd[1]); // close the write end of the pipe
    } 
    else 
    { // Parent process

        close(fd[1]); // close the write end of the pipe because we are going to use the read end
        int arr[10]; // array to hold the received numbers
        int n, i, sum = 0; 
        
        if (read(fd[0], &n, sizeof(int)) < 0) // read the number of elements from the pipe
        {
            return 5;
        }
        printf("Received n = %d\n", n); // print the number of elements
        for (i = 0; i < n; i++)
        {
            if (read(fd[0], &arr[i], sizeof(int)) < 0) // read one element from the pipe
            {
                return 6; // Return error if the read operation fails
            }
        }
        printf("Received array\n"); // print the received array
        
        close(fd[0]); // close the read end of the pipe
        for (i = 0; i < n; i++) // sum up the numbers in the array
        {
            sum += arr[i];
        }
        printf("Result is %d\n", sum); // print the result
        wait(NULL); // wait for the child process to finish
    }
    
    return 0;
}