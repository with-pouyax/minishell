#include "../minishell.h"


#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*
Exit Status Handling:
WIFEXITED(status): Checks if the child exited normally.
WEXITSTATUS(status): Retrieves the exit code of the child if it exited normally.
WIFSIGNALED(status): Checks if the child was terminated by a signal.
WTERMSIG(status): Retrieves the signal number if the child was terminated by a signal.
WIFSTOPPED(status): Checks if the child was stopped (e.g., by a signal like SIGSTOP).
WSTOPSIG(status): Retrieves the signal number if the child was stopped.
*/
void execute_parent(t_shell_data *shell)
{
    t_pid_node *current;
    pid_t       pid;
    int         status;

    current = shell->pid_list;
    while (current != NULL)
    {
        pid = current->pid;
        if (waitpid(pid, &status, 0) == -1)
            perror("waitpid failed");
        else
        {
            if (WIFEXITED(status))
                printf("\n\nChild process %d exited with status %d\n", pid, WEXITSTATUS(status));
            else if (WIFSIGNALED(status))
                printf("Child process %d was terminated by signal %d\n", pid, WTERMSIG(status));
            else if (WIFSTOPPED(status))
                printf("Child process %d was stopped by signal %d\n", pid, WSTOPSIG(status));
        }
        current = current->next;
    }
    clear_pid_list(shell);
}

void clear_pid_list(t_shell_data *shell)
{
    t_pid_node *current = shell->pid_list;
    t_pid_node *next_node;

    while (current != NULL)
    {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    shell->pid_list = NULL;
}


