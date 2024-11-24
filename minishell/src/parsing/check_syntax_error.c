#include "../minishell.h"

int check_syntax_error(t_shell_data *shell, char *error_message)
{
    if (error_message)
    {
        ft_putstr_fd(error_message, STDERR_FILENO);
        free(shell->input);
        free(shell->full_input);
        shell->input = NULL;
        shell->full_input = NULL;
        shell->exit_status = 2;
        return (1); // Skip further processing
    }
    return (0); // Continue processing
}