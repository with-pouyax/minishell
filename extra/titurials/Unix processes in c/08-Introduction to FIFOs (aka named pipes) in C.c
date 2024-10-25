/* 
- one thing was clear by last leeason, that pipes could only be used by processes that are in the same hierarchy.
- below I have file descriptors, and these file descriptors are very special in the scence that, if we do fork the process,
they are going to be inside the operating system itself, they are going to be copied over from this process to the child process.
- so they can be used in that process and closed on theire own.
- so these are sort of special entities(موجودیت, نهاد). 
- we can read have a pipe between two processes that are not in the same hierarchy. can we ?
- there is another feature similar to pipes in unix systems, that is called FIFOs.
- FIFOs are basically files types to which you can read or write from any process that we opned.
 */

int main(int argc, char *argv[])
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        return 1;
    }
    return 0;
}

/* 
- lets create a FIFO, and we will see how we can use it.
- instead of unistd.h, we need to include sys/types.h and sys/stat.h
- first we need to create a FIFO file, to create we just need to call mkfifo() function.
- this guy takes in the path to the file, that wa want to create.
- First argument is the name of the file, and the second argument is the permissions of the file.
- now if I go to my code directory, I will see a file called myfifo.
 */

int main(int argc, char *argv[])
{
    mkfifo("myfifo", 0777);
    return 0;
}

/* 
-there is few more important things we have to do here besides just calling mkfifo().
1- check status code of mkfifo() function.  -1 if something went wrong. 0 if everything went well.
2- with errno, we can check what went wrong. but like below, we continue the code, if the file already exists.

- unlike pipes, we have after creating the FIFO, we can open it and read and write from it.
- attention: open() function returns, something very very important and that is file descriptor.
- now we can use this, the same way we used the file descriptor from the pipe.
- but now if we run the code, the code will hang, why ?
-
 */

int main(int argc, char *argv[])
{
    if (mkfifo("myfifo", 0777) == -1)
    {
        if (errno != EEXIST)
        {
            printf("could not create fifo\n");
            return 1;
        }
    }
    int fd = open("myfifo", O_WRONLY);
    int x = 97;
    if (fd, &x, sizeof(x)) == -1)
    {
        printf("could not write to fifo\n");
        return 2;
    }
    close(fd);
    return 0;
}

/* 
- so I gonna add some printf() to see what is going on.
- so now if I execute the code, I will see :
openning ...
- and the code will hang. 
what is the problem ?
- the reason behind that is a really special functionality of FIFOs. and that is something that's written in the linux manual page of open().
- in the FIFOs section, it says if we open a FIFO for writing, the open() call blocks until another process comes along and opens the same
FIFO for reading and then, when the other process opens it for reading, the process that opened it for writing will continue.
- if I just open another terminal and read myfifo, using cat myfifo, I will see that my code will continue executing on the other terminal.
- so the result will be :
Openning ...
Openned
Written
Closed
- so as you see, you always have to have both ends open at the same time, that's their special property. if we don do that, one
of the oppen() calls is gonna hang.
- or if I open the same FiFO for multiple times for writing, all of those open() calls are gonna hang. because there is no process
that is opening it for reading.

as you see when we cat myfifo, I received a, because 97 is the ASCII code for a.
- before going to learn how FIFOs communicate we had to learn this.
- also if I cat myfifo without writing to it, the cat command will hang.
- if we open the FIFO O_RDWR (read and write), if I luanch the code, it gonna work, because we are opening it for reading and writing.
- one more remark here is, you don't have to actually call mkfifo() from a c file, you can first create the FIFO file myself using
mkfifo command in the terminal like this:
mkfifo myfifo
- one last thing is we have to check what open() returns, if it returns -1 we have to return 1 for example.

 */

