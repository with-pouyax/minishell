/* 
- we already looked at how to send signals, how to handle signals, but we only
worked with signals that are doing something special.
- now we will look at how to send signals that are not doing anything special.
- lets first have a simple program:
- we want a simple question to be asked, the user gonna have to answere it, if it 
takes more that 5 seconds to answer, I want user recieve a hint.
- How can we do that using 2 processes ?
- here scanf waits for the user to input, how can it also wait for 5 seconds ?
 
 */

int main(int argc, char *argv[])
{
    int pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }
    if (pid == 0)
    {

    }
    else
    {
        int x;
        printf("What is the result of 3 * 5 ?\n");
        scanf("%d", &x);
        if (x == 15)
        {
            printf("Correct\n");
        }
        else
        {
            printf("Wrong\n");
        }
    }
    return 0;
}

/* 
- we do that using child process.
- now if we lunch this program and answere it fast, I will get :
What is the result of 3 * 5 ? 15
Correct!
HINT => Remember taht multiplication is just a fancy way of adding numbers

- although I answere it correctly, I still get the hint. why ? how can we fix it ?

 */

void handle_sigusr1(int signum)
{
    printf("HINT => Remember taht multiplication is just a fancy way of adding numbers\n");
}


int main(int argc, char *argv[])
{
    int pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }
    if (pid == 0)
    {
        sleep(5);
        kill(getppid(), SIGUSR1);
    }
    else
    {
        struct sigaction sa = {0}; // here all the fields of sa are set to 0.
        sa.sa_flags = SA_RESTART; // because we're using scanf and SIGUSR1.
                                  // if we had no scanf, we wouldn't need this flag.
        sa.sa_handler = &handle_sigusr1;
        sigaction(SIGUSR1, &sa, NULL);

        int x;
        printf("What is the result of 3 * 5 ?\n");
        scanf("%d", &x);
        if (x == 15)
        {
            printf("Correct!\n");
        }
        else
        {
            printf("Wrong!\n");
        }
        wait(NULL);
    }
    return 0;
}

/* 
- the quick fix is to move the x variable and make it global, and give the value of 0
to it when we declare it.
- then at the handle_sigusr1 function, we check if x is 0, then we print the hint.
- now if we run the program, and answere it correctly, we will not get the hint.
 */


int x = 0;

void handle_sigusr1(int signum)
{
    if (x == 0)
        printf("HINT => Remember taht multiplication is just a fancy way of adding numbers\n");
}


int main(int argc, char *argv[])
{
    int pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }
    if (pid == 0)
    {
        sleep(5);
        kill(getppid(), SIGUSR1);
    }
    else
    {
        struct sigaction sa = {0}; // here all the fields of sa are set to 0.
        sa.sa_flags = SA_RESTART; // because we're using scanf and SIGUSR1.
                                  // if we had no scanf, we wouldn't need this flag.
        sa.sa_handler = &handle_sigusr1;
        sigaction(SIGUSR1, &sa, NULL);

        printf("What is the result of 3 * 5 ?\n");
        scanf("%d", &x);
        if (x == 15)
        {
            printf("Correct!\n");
        }
        else
        {
            printf("Wrong!\n");
        }
        wait(NULL);
    }
    return 0;
}