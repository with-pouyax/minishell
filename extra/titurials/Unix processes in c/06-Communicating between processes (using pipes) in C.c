/* 
- How to communicate between processes, without copying the memory over ?
- there is a functionality in C called pipe.
- pipe is a file that has sort of a buffer, that is saved in the memory, and you can
write and read from it.
- so in this case, if we want to open a pipe, what we have to do is to call the
function called pipe, and this guy takes in an array of two integers, and does
two intigers are really important, because they are the file descriptors for this
pipe.
- a file descriptor is basically a key for access to a file.
- ok first we have to create an array of two integers.
- then we call the pipe function, and we pass in the array of two integers.
- if the pipe function returns -1, that means that an error occured.
- if the pipe function returns 0, that means that the pipe was created successfully.
- now that we opened the pipe successfully, we can now fork.
- why we can only do this in this order ?
- the thing is when we fork, the fd array is copied over to the child process.
- and the file descriptors inside the fd array are kind of nice, because aside just
getting copied over, behind the scenes, those file descriptors are assigned to the
newly created process. so they get inherited.
- what does that mean ?
- that means that if you for example close that file descriptor at one point, in a
process, well that file descriptor in the other process will remain open.
- they sort of have their independent life.
- so in our case fd array will be copied over and inherited as well, so they can
be open or closed independently.
- so now we will continue writing a program that just asks the user to input
a number, and it sends that to other process.(to the parent process from the child
process), and the parent process will print that number.
- before start let's understand what is each file descriptor in the fd array.
- the first file descriptor is for reading, and the second file descriptor is for writing.


 */
int main(int argc, char *argv[])
{
    int fd[2]; 
    if (pipe(fd) == -1) // this makes a pipe, and it saves the file descriptors in the array fd
    {
        printf("an error occured with opening the pipe\n");
        return 1;
    }
    int id = fork();
    if (id == -1)
    {
        printf("an error occured with fork\n");
        return 1;
    }
    if (id == 0) // if we are in the child process
    {
        close(fd[0]); // we close the reading file descriptor because we are not going to read from the pipe
        int x; // we will need a variable to store the number that the user inputs
        printf("Input a number: "); // we ask the user to input a number
        scanf("%d", &x); // we store the number in the variable x
        if (write(fd[1], &x, sizeof(int)) == -1) // we write the user input to the pipe
        {
            printf("an error occured with writing to the pipe\n");
            return 1;
        }
    
            
        close(fd[1]); // we close the writing file descriptor because we are done writing to the pipe
    }
    else // if we are in the parent process 
    {
        close(fd[1]); // we close the writing file descriptor because we are not going to write to the pipe
        int y; // we will need a variable to store the number that we read from the pipe
        if (read(fd[0], &y, sizeof(int)) == -1) // we read the number from the pipe
        {
            printf("an error occured with reading from the pipe\n"); 
            return 1;
        }
        printf("The number is: %d\n", y); // we print the number that we read from the pipe
        close(fd[0]); // we close the reading file descriptor because we are done reading from the pipe
        printf("Got from child process %d\n", y); // we print the number that we read from the pipe
    }
        
    return 0;
}