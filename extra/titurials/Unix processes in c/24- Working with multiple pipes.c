# Working with multiple pipes
- we gonna start with a simple exercise:
- we want to create 3 processes (2 forks). we call one of them a and the other b.
- we want to pass an int arround all the processes.
- I want my main create an x with a value of 0 and pass it a, a will increment it by 5 and pass it to b, b will increment it by 5 and pass it back to main.
- How many pipes do we need? and which one do we need to close?
- for starters, we need definintly 3 pipes, because pipes are unidirectional, and they represent a link between 2 processes and in our case, we have 3 arrows, in every case we need to send one integer to the other process., so we can say we need 3 pipes, But where should we open those pipes? 
- for simplicity sake, I'm gonna open all the pipes in the parrent process.
- how many file descriptors does the parent process have?
- well we have 3 pipes, and each pipe has 2 file descriptors, so we have 6 file descriptors in total in the parent process.
- now the main key about this, is that those file descriptors are gonna be inherited by the child processes.
- so each of a and b will have copy of those 6 file descriptors of their own.
- what does that mean for our program ? well that means that we are gonna have to call close() function 18 times in the whole c source code.
- This is the most important part, when dealing multiple pipes, with this inheritance of file descriptors and the need to close them.
- now if I lunch the program, I get :

```bash
Result: 10
```

```c
    int main( int argc, char *argv[] )
    {
        int fd[3][2]; /* we make a 2D array of file descriptors, 3 rows and 2 columns, it mean this array will hold 3 elements, 
        each element is holding 2 file descriptors, one for reading and one for writing. */
        
        int i;
        for( i = 0; i < 3; i++ ) // we iterate 3 times
        {
            if( pipe( fd[i] ) < 0 ) // and we open a each time a pipe, and we store the file descriptors in the array fd array.
            {
                /* - normally in this case, we have to treat the error, like for example, if 2 of the pipes are already opened, 
                and the third one fails, we should close the first 2 pipes, and then return an error message.
                - but for simplicity sake, I'm just gonna return an error message here. */
                perror( "pipe" );
                return 1;
            }

            int pid1 = fork(); // we fork the first time, we call the child process a.
            if( pid1 < 0 )
            {
                // - same thing here, we should close the pipes that are already opened, and return an error message.
                perror( "fork" );
                return 2;
            }
            if( pid1 == 0 )
            {
                // - this is the child process a
                /* we have to decide which pipes are gonna be used for reading and writing and which ones we have to close straight away. */
                close( fd[0][1] ); // we close the writing end of the first pipe, because we are gonna read from it.
                close( fd[1][0] ); // we close the reading end of the second pipe, because we are gonna write to it.
                close( fd[2][0] ); // we close the reading end of the third pipe, because we dont do anything with 3rd pipe in this process.
                close( fd[2][1] ); // we close the writing end of the third pipe, because we dont do anything with 3rd pipe in this process.
                int x;
                if (read( fd[0][0], &x, sizeof( int ) ) < 0 ) // we read from the first pipe, and store the value in x.
                {
                    perror( "read" );
                    return 3;
                }
                x += 5; // we increment x by 5.
                if( write( fd[1][1], &x, sizeof( int ) ) < 0 ) // we write x to the second pipe.
                {
                    perror( "write" );
                    return 4;
                }
                close( fd[0][0] ); // we close the reading end of the first pipe, that we already read from.
                close( fd[1][1] ); // we close the writing end of the second pipe, that we already wrote to.
                return 5; // we return 0, because we are done with this process.
            }

            int pid2 = fork(); // we fork the second time, we call the child process b.
            if( pid2 < 0 )
            {
                // - same thing here, we should close the pipes that are already opened, and return an error message.
                perror( "fork" );
                return 6;
            }
            if( pid2 == 0 )
            {
                // - this is the child process b
                /* we have to decide which pipes are gonna be used for reading and writing and which ones we have to close straight away. */
                close( fd[0][0] ); // we close the reading end of the first pipe, because we dont do anything with 1st pipe in this process.
                close( fd[0][1] ); // we close the writing end of the first pipe, because we dont do anything with 1st pipe in this process.
                close( fd[1][1] ); // we close the writing end of the second pipe, because we are gonna read from it only. 
                close( fd[2][0] ); // we close the reading end of the third pipe, because we are gonna write to it. 
                int x;
                if (read( fd[1][0], &x, sizeof( int ) ) < 0 ) // we read from the second pipe, and store the value in x.
                {
                    perror( "read" );
                    return 7;
                }
                x += 5; // we increment x by 5.
                if( write( fd[2][1], &x, sizeof( int ) ) < 0 ) // we write x to the third pipe.
                {
                    perror( "write" );
                    return 8;
                }
                close( fd[1][0] ); // we close the reading end of the second pipe, that we already read from.
                close( fd[2][1] ); // we close the writing end of the third pipe, that we already wrote to.
                return 9; // we return 0, because we are done with this process.
            }

            // parent process
            close( fd[0][0] );  
            close( fd[1][0] );
            close( fd[1][1] );
            close( fd[2][1] );
            int x = 0;
            if ( write( fd[0][1], &x, sizeof( int ) ) < 0 )
            {
                perror( "write" );
                return 11;
            {
            if ( read( fd[2][0], &x, sizeof( int ) ) < 0 )
            {
                perror( "read" );
                return 12;
            }
            printf( "Result: %d\n", x );
            close( fd[0][1] );
            close( fd[2][0] );

            wait( pid1, NULL, 0 ); // we wait for the first child process to finish.
            wait( pid2, NULL, 0 ); // we wait for the second child process to finish.
      
            return 0;
        }
}
```

- what we wanted to realize in this video, all the file descriptors opened in a process when we fork, they get duplicated in the child processes, thats why we end up with 18 file descriptors.
- as a home work, try to have instead of 3 processes, n processes, and try to pass