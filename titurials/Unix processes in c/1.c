#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>  // For wait()

int main(int argc, char *argv[])
{
    printf("parent process ID: %d\n", getppid());
    printf("current process ID: %d\n", getpid());
    printf("we have our first fork after this line\n");

    int id = fork();

    if (id > 0) {
        // Parent process waits for the first child
        wait(NULL);
    }

    printf("ID: %d\n", id);
    printf("parent process ID: %d\n", getppid());
    printf("current process ID: %d\n", getpid());

    printf("-----------------\n");

    int id2 = fork();

    if (id2 > 0) {
        // Parent process waits for the second child
        wait(NULL);
    }

    printf("ID2: %d\n", id2);
    printf("parent process ID2: %d\n", getppid());
    printf("current process ID2: %d\n", getpid());

    return 0;
}



/* #include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    printf("parent process ID: %d\n", getppid());
    printf("current process ID: %d\n", getpid());
    printf("we have our first fork after this line\n");

    int id = fork();
    printf("ID: %d\n", id);

    printf("parent process ID: %d\n", getppid());
    printf("current process ID: %d\n", getpid());

    if (id > 0) {
        // Parent process: wait for the child to finish
        wait(NULL);
    }

    return 0;
} */

