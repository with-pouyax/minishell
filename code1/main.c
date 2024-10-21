#include "minishell.h"

// Initialize shell state
void init_shell(t_shell *shell, char **envp)
{
    shell->input = NULL;
    shell->exit_status = 0;
    shell->commands = NULL;
    shell->envp = envp;
}

// Cleanup shell state
void cleanup_shell(t_shell *shell)
{
    if (shell->input)
        free(shell->input);
}

// Main function
int main(int argc, char **argv, char **envp)
{
    t_shell shell;

    (void)argc;
    (void)argv;
    init_shell(&shell, envp);
    listen_for_input(&shell);
    cleanup_shell(&shell);

    return 0;
}
