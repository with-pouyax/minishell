/* 
- in previous lesson we looked at, how to send a process to the background
and how to bring it back to the foreground.
- In this lesson, we will look at how to handle signals and namely (بعبارت دیگر) how to
prevent the process to be sent to the background at all ?
- first we have this code:
 */

int main(int argc, char *argv[])
{
    int x;
    printf("Enter a number: ");
    scanf("%d", &x);
    printf("Result %d * 5 = %d\n", x, x * 5);
    return 0;
}

/* 
- how can we make it so that, whenever I launch this program and then ctrl + z, it doesn't say "stopped" and it doesn't go to the background ?
- the first step is to instantiate a struct called sigaction and we will call it sa.
- then we will set the sa.sa_handler to the address of a function that we will define later called handle_sigtstp.
- sa_handler is a function pointer that will be called whenever the signal is received.
- now we make the sa_handler function and what I want to do here is I want to handle sigtstp signal, that we should receiving to actually
stop the process, but now I want to change the behavior of this signal.
- next stop is we have too hook this sigaction to the proper signal, so we will call sigaction(SIGTSTP, &sa, NULL).
- the first argument is the signal that we want to handle, 
- the second argument is the struct that we want to handle the signal with,
- the third argument is if we had another sigaction struct set before, it would save it inside the third argument, so we can use it later.
- we also write sa.sa_flags = SA_RESTART; that is just because we're using the combination of SIGTSTP and scanf, because they are doing
something strange, behind the scenes, it doesn't work well if we don't provide this flag. 
- now we run this and we press ctrl + z, and it says "I won't stop".
 */

void handle_sigtstp(int signum)
{
    printf("I won't stop\n");
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = &handle_sigtstp; 
    sa.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sa, NULL);

    int x;
    printf("Enter a number: ");
    scanf("%d", &x);
    printf("Result %d * 5 = %d\n", x, x * 5);
    return 0;
}

/* 
- now lets take a look at another function, very similar to sigaction called signal.
- this function only does take two arguments, the first one is the signal that we want to handle and the second one is the
function that we want to handle the signal with.
- now if we try to run this code, we will see :
- the exact same behavior.

- why we use sigaction instead of signal ?
- because in man page of signal, it says that signal is not portable, so it's better to use sigaction.
- so in order your code works on all systems, and all unix-like systems, it's better to use sigaction.
 */

void handle_sigtstp(int signum)
{
    printf("I won't stop\n");
}

int main(int argc, char *argv[])
{
    //struct sigaction sa;
    //sa.sa_handler = &handle_sigtstp; 
    // sa.sa_flags = SA_RESTART;
    // sigaction(SIGTSTP, &sa, NULL);

    signal(SIGTSTP, &handle_sigtstp);
    int x;
    printf("Enter a number: ");
    scanf("%d", &x);
    printf("Result %d * 5 = %d\n", x, x * 5);
    return 0;
}

/* 
- what if I want to instead when I type fg to bring the process 
back to the foreground that prompt again ? (because after we ctrl + z, and then fg, it
doesn't prompt us to enter a number again but we can enter a number and it will work)
 */

void handle_sigtstp(int signum)
{
    printf("I won't stop\n");
    fflush(stdout); // this is to make sure that the output is printed right when we
    // press ctrl + z

}

void handle_sigcont(int signum)
{
    printf("Input a number: ");
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = &handle_sigcont; 
    sa.sa_flags = SA_RESTART;
    sigaction(sigcont, &sa, NULL);

    signal(SIGTSTP, &handle_sigtstp);
    int x;
    printf("Enter a number: ");
    scanf("%d", &x);
    printf("Result %d * 5 = %d\n", x, x * 5);
    return 0;
}

/* 
- we might notice a diffrence here, with SIGTSTP, when we change the handler it did 
no longer stop, but with SIGCONT it continues its execution, why ?
- because SIGCONT is sort of special in the scence that, while you can block it, you
can handle it differently, it is still going to continue the execution of the process,
it's still going to do same thing as before. except it is gonna do some extra stuff
that we added in the handle_sigcont function.
- by some signals we are not allowed to handle them ourselves.
 */
    
