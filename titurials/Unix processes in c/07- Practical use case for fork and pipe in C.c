/* 
- so we gonna have an array of integers.
- we want to devide the array into two parts.
- some up one half of the array with one process,
and sum up the other half of the array with another process.
- so at the end both processes will have a partial sum of the array,
so at the end one of the processes will send its partial sum to the other process,
and the other process will add its partial sum to the partial sum that it received,
and then it prints the total sum of the array.
 */
int main(int argc, char *argv[])
{
    int arr[] = {1, 2, 3, 4, 1, 2}; //
    int arr_size = sizeof(arr) / sizeof(int); // here we get the size of the array
    int start; 
    int end;
    int fd[2];
    if (pipe(fd) == -1) // here we create a pipe
    {
        printf("an error occured with opening the pipe\n");
        return 1;
    }
    int id = fork(); // here we create a child process
    if (id == -1)
    {
        printf("an error occured with fork\n");
        return 1;
    }
    if (id == 0) // if we are in the child process
    {
        start = 0; // we start from the beginning of the array
        end = arr_size / 2; // we end at the middle of the array
    }
    else // if we are in the parent process
    {
        start = arr_size / 2; // we start from the middle of the array
        end = arr_size; // we end at the end of the array
    }

    int sum = 0;
    int i;
    for (i = start; i < end; i++) // here we sum up the elements of the array
    {
        sum += arr[i]; 
    }
    printf("calculated partial sum: %d\n", sum); // here we print the partial sum

    if (id == 0) // if we are in the child process
    {
        close(fd[0]); // we close the read end of the pipe
        if (write(fd[1], &sum, sizeof(sum)) == -1) // here we write the partial sum to the pipe
        {
            printf("an error occured with writing to the pipe\n");
            return 1;
        }
        close(fd[1]); // we close the write end of the pipe
    }
    else // if we are in the parent process
    {
        close(fd[1]); // we close the write end of the pipe
        int sum_from_child; // here we declare a variable to store the partial sum
                            // that we will read from the pipe 
        if (read(fd[0], &sum_from_child, sizeof(sum_from_child)) == -1) // here we read 
        // the partial sum from the pipe
        { 
            printf("an error occured with reading from the pipe\n");
            return 1;
        }
        close(fd[0]); // we close the read end of the pipe
        int total_sum = sum + sum_from_child; // here we calculate the total sum
        printf("total sum: %d\n", total_sum); // here we print the total sum
        wait(NULL); // here we wait for the child process to finish
        }

    
    return 0; // here we return 0
}

    /* 
    --Homework--
    - instead of having 2 processes, we want to have 3 processes.
    - so two child processes will sum up the array, and then they will send their 
    partial sum to the parent process. and the parent process will sum up the partial
    sums and print the total sum of the array.
     */