// src/ft_env.c
#include "internal_commands.h"

int		ft_env(t_shell_data *shell,t_command *cmd)
{
	t_token	*token;
	int		arg_count;
	int		i;

	token = cmd->token_list->next; // Skip the command token
	arg_count = 0;

	// Count the number of arguments provided to env
	while (token)
	{
		arg_count++;
		token = token->next;
	}

	if (arg_count > 0)
	{
		// env does not accept any arguments in this implementation
		ft_putstr_fd("minishell: env: too many arguments\n", STDERR_FILENO);
		shell->exit_status = 1;
		return (1);
	}

	// No arguments provided; print all environment variables
	i = 0;
	while (shell->envp[i])
	{
		ft_putstr_fd(shell->envp[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}

	shell->exit_status = 0;
	return (0);
}
