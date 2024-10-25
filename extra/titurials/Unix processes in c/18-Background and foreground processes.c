/*
- before we go any further with signals, lets talk about
concept of putting a process in the background and bringing it back to the foreground.

- to do that we are gona make a very simple program that stores a variable x and then
prints a prompt says input number, then we read a number from the user and store it
in x, then we print a simple operation

- so now if I type 10, it will print 10 * 5 = 50
  */

 int main(int argc, char *argv[])
 {
     int x;
     printf("Input number: ");
     scanf("%d", &x);
     printf("Result: %d * 5 = %d\n", x, x * 5);
     return 0;
}

/* 
- bring a process to the background means stopping the process, and to do if I go to
Terminal execute the program, but instead of typing 10, I press ctrl + z, you will see
that the process will be stopped and you will see:
has stopped
- has stopped doesn't mean the process has terminated, terminated means the program
reached an exit call or or it reached the return statement inside it's main,
so it basically finished it's execution, is done all the memory as it has been freed
and were done.
- but stopped a proccess is more like pausing the process, it's like in a state where
it's stuck at a certain line of code, for whatever reason, and it waits for a signal
to continue it's execution.
- in fact when I hit here ctrl + z, a signal was sent from my terminal this process and
we are saying, okay you're gonna stop your execution, you're gonna pause your execution
and it did indeed say stopp so now I can bring it back from the state by either sending
in a sigcont or just by typing fg, fg means foreground and it does the same thing as
sending a sigcont signal. you will notice it says:
send job 1, "./main" to
- it means that the process has been brought back to the foreground, and now it's
executing again, job 1 is the job number,which means that it's the first job that
has been stopped, and now it's the first job that has been brought back to the foreground.
and job 1 was ./main, now it actually sends the SIGCONT signal to it and it continues
it's execution, so now it's actually waiting for us to input a number.
- this is how we can move a process to the background and bring it back to the foreground
- lets look at wahts happening behind the scenes, with those signals.
-lets draw a timeline of all the lines of code that we have in the program added as
18.01- diagram
-
 */