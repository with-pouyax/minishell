/* 
- lets solve two small problems using FIFOs.
1- it is simply gonna generate some numbers and otherone gonna read them from the product generated them, and
print out the sum.

- we gonna create the FIFO file file from Terminal this time called sume.

 */

int main(int argc, char *argv[])
{
    int arr[5];
    srand(time(NULL)); // seed the random number generator
    int i;
    for (i = 0; i < 5; i++)
    {
        arr[i] = rand() % 100; // generate a random number between 0 and 99
    }
    int fd = open("sume", O_WRONLY);
    if (fd == -1)
    {
        printf("could not open the FIFO file\n");
        return 1;
    }

    for (i = 0; i < 5; i++) 
    {
        if write(fd, &arr[i], sizeof(arr[i]) == -1) // write the number to the FIFO file
        {
            printf("could not write to FIFO file\n");
            return 2;
        }
        printf("wrote %d to the FIFO file\n", arr[i]); // print out the number that was written
    }
    close(fd);

    return 0;
}

/* 
- we also gonna have other program
- now that I have this two programs, first I have to compile them both.
- we can do it by prssiing ctrl+shift+b.
-then I open two terminals, I first run first program, and we will see that it will hang (blocked).
- then I run the second program, and I see both programs are finished.
and we recieved :
main1       main2
_______     _______
wrote 8     received 8
wrote 18    received 18
wrote 10    received 10
wrote  89   received 89
wrote 24    received 24
so we know that inside arr in second program we stored all the. so now we sum them up.
 */

int main(int argc, char *argv[])
{
    int arr[5]; // array to store the numbers that are read from the FIFO file
    int fd = open("sume", O_RDONLY); // open the FIFO file for reading
    if (fd == -1)
    {
        printf("could not open the FIFO file\n");
        return 1;
    }

    int i;
    for (i = 0; i < 5; i++)
    {
        if read(fd, &arr[i], sizeof(arr[i]) == -1) // read the number from the FIFO file
        {
            printf("could not read from FIFO file\n");
            return 2;
        }
        printf("read %d from the FIFO file\n", arr[i]); // print out the number that was read
    }
    close(fd);

    int sume = 0;
    for (i = 0; i < 5; i++)
    {
        sume += arr[i];
    }
    printf("sume = %d\n", sume); 
    return 0;
}

/* 
--- Homework ---
- instead of print the sum on main2, send it back to the other program and print it there.
 */


/* 
- we can do an Optimization i our code, regarding reading and writing, we are doing it fairly inefficiently.
- since arrays are number of bytes next to each other, can we write them all at once, instead of
writing them one by one ?
-
 */
// main1
int main(int argc, char *argv[])
{
    int arr[5];
    srand(time(NULL)); 
    int i;
    for (i = 0; i < 5; i++)
    {
        arr[i] = rand() % 100; 
        printf("Generated %d\n", arr[i]);
    }

    int fd = open("sume", O_WRONLY);
    if (fd == -1)
    {
        printf("could not open the FIFO file\n");
        return 1;
    }
    
    if write(fd, arr, sizeof(int) * 5) == -1) 
    {
        printf("could not write to FIFO file\n");
        return 2;
    }
    close(fd);
}

// main2

int main(int argc, char *argv[])
{
    int arr[5]; // array to store the numbers that are read from the FIFO file
    int fd = open("sume", O_RDONLY); // open the FIFO file for reading
    if (fd == -1)
    {
        printf("could not open the FIFO file\n");
        return 1;
    }

    int i;
    
    if (read(fd, &arr[i], sizeof(int) * 5) == -1) 
    {
            printf("could not read from FIFO file\n");
            return 2;
    }
    close(fd);

    int sume = 0;
    for (i = 0; i < 5; i++)
    {
        sume += arr[i];
    }
    printf("sume = %d\n", sume); 
    return 0;
}

/* 
- the main takeaway from this is that you can send batches of data, if you have an array
you can send the whole array at once.
 */