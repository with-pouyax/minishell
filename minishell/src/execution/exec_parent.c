#include "../minishell.h"

void wait_for_all_children(t_shell_data *shell)
{
    t_command *cmd;
    int        status;

    cmd = shell->commands;
    while (cmd)
    {
        if (cmd->pid > 0) // Check if this command has a valid PID
        {
            if (waitpid(cmd->pid, &status, 0) < 0)
                perror("waitpid failed");
            if (WIFEXITED(status))
                shell->exit_status = WEXITSTATUS(status); // Update exit status
        }
        cmd = cmd->next;
    }
}
