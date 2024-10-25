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
    int arr[] = {1, 2, 3, 4, 1, 5};
    int arr_size = sizeof(arr) / sizeof(int);
    int start;
    int end;
    int fd[2];
    int id1;
    int id2;

    pipe(fd);
    
    id1 = fork();
    if (id1 == -1)
    {
        printf("problem making fork\n");
        return 1;
    }
    if (id1 != 0)
    {
        id2 = fork();
        if (id2 == -1)
        {
            printf("problem making fork\n");
            return 2;
        }
    }
    if (id1 == 0)
    {
        start = 0;
        end = arr_size / 2;
    }
    else if (id2 == 0)
    {
        start = arr_size / 2;
        end = arr_size;
    }

    int sume;
    while(start < end)
    {
        sume += arr[start];
        start++;
    }
    if (id1 == 0)
    {
        printf("sume1 = %d\n", sume);
        close(fd[0]);
        write(fd[1], &sume, sizeof(sume));
        close(fd[1]);
    }
    else if (id2 == 0)
    {
        printf("sume2 = %d\n", sume);
        close(fd[0]);
        write(fd[1], &sume, sizeof(sume));
        close(fd[1]);
    }
    else
    {
        close(fd[1]);
        int sume1;
        int sume2;
        int total_sume;
        
        read(fd[0], &sume1, sizeof(sume1));
        read(fd[0], &sume2, sizeof(sume2));
        total_sume = sume1 + sume2;
        close(fd[1]);
        printf("Total_sume = %d\n", total_sume);

        wait(NULL);
        wait(NULL);
    }
}