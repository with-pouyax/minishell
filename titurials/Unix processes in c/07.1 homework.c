/* 
    --Homework--
    - instead of having 2 processes, we want to have 3 processes.
    - so two child processes will sum up the array, and then they will send their 
    partial sum to the parent process. and the parent process will sum up the partial
    sums and print the total sum of the array.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main (void)
{
    int arr[] = {1, 2, 3, 4, 1, 2};
    int arr_size = sizeof(arr) / sizeof(int);
    int start;
    int end;
    int fd[2];
    
    int id1 = fork();
    if (id1 == -1)
    {
        printf("problem making fork\n");
    }
    if (id1 != 0)
    {
        int id2 = fork();   
    }
}