#include "minishell.h"

void	handle_tokenization_error(t_shell_data *shell, int error_flag)
{
	if (error_flag == 1)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
			STDERR_FILENO);
		shell->exit_status = 2;
	}
	else if (error_flag == 2)
	{
		ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
		cleanup();
		exit(EXIT_FAILURE);
	}
}

void	tokenize_input_error(t_shell_data *shell, int error_flag)
{
	free_commands(shell);
	shell->commands = NULL;
	shell->error_flag = error_flag;
}