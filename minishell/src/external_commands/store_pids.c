#include "../minishell.h"

void store_pids(t_shell_data *shell, pid_t pid)
{
    t_command *current;

    current = shell->commands;
    while (current)
    {
        if (current->pid == 0)
        {
            current->pid = pid;
            return;
        }
        current = current->next;
    }
    // If we reach here, something is wrong (no available command node)
    fprintf(stderr, "Error: Unable to store PID %d\n", pid);
}
