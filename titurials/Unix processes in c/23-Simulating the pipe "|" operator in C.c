/* 
-we already know that with exec family of functions we can run a program but not a
script or shell command.
- so we can't run a command like ping -c 5 google.com | grep rtt using exec functions.

- lets analyze the command ping -c 5 google.com | grep rtt:
- so here we are executing two programs, ping and grep. and other things are arguments
to these programs.
- this two programs are linked by a pipe operator "|".
- it is the same as the pipe we use in C programming, except that it does a few more
things besides just opening a pipe.
- so if we think of this pipe, opening a pipe, that also means that we are actually
opening a pipe taht has two ends and those two ends in this case (our cpommand) are
routed(هدایت شده) in a certain way.
- so we know that when we execute ping -c 5 google.com, it is printing out some
information to the standard output.
- but here it didn't print out anything and it printed out whatever grep found.
- so that mean the output of left program is actually routed through the pipe.
- so ping wrote its output instead of the standard output to this pipe.
- But how grep knows that it should look at those lines?
- that is because also the standard input of grep is routed to the pipe and because of
that since grep knows how to handle standard input, it is not waiting for keyboard 
input, it is actually reading from the pipe.
- now I will draw a diagram to show how this pipe is actually working on other file.

 */