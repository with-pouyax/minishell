// src/ft_env.c
#include "../internal_commands.h"

/*
1. Skip the command token
2. while (token) :  Count the number of arguments provided to env
3. while (shell->envp[i]) : No arguments provided; print all env_ variables
*/

void	print_env(t_shell_data *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		ft_putstr_fd(shell->envp[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
}

int	ft_env(t_shell_data *shell, t_command *cmd)
{
	int	arg_count;
	t_token	*token;

	token = cmd->token_list->next;
	arg_count = 0;
	while (token)
	{
		arg_count++;
		token = token->next;
	}
	if (arg_count > 0)
	{
		shell->exit_status = 1;
		return (1);
	}
	print_env(shell);
	shell->exit_status = 0;
	close(shell->saved_stdin);
    close(shell->saved_stdout);
	return (0);
}
