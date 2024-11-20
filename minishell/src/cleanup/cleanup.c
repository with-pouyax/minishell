#include "minishell.h"

void free_envp(t_shell_data *shell)
{
    int i;

    if (!shell->envp)
        return;

    for (i = 0; shell->envp[i]; i++)
    {
        free(shell->envp[i]);
    }
    free(shell->envp);
    shell->envp = NULL;
}

void cleanup(t_shell_data *shell)
{
    free(shell->input);
    shell->input = NULL;
    free(shell->full_input);
    shell->full_input = NULL;
    free_commands(shell);
    free_envp(shell);
}
