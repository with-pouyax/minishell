/* 
- in main function arguments.
- argc stands for argument count and it is an integer that represents the number of arguments
that are passed to the program.
- argv stands for argument vector, and whemevr we have a vektor or an array, we kind
of want to have a count so I can know how many elements are in the array.
- this is why we have 2 arguments in the main function.
with code bellow I can print all the arguments that are passed to the program.
 */
#include <stdio.h>

int main(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    return 0;
}