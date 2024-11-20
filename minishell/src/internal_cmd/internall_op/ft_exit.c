// ft_exit.c
#include "internal_commands.h"

int	ft_exit_shell(t_shell_data *shell, t_command *cmd)
{
	t_token	*token;
	int		exit_status;

	token = cmd->token_list->next; // Skip the command token
	exit_status = shell->exit_status;

	printf("exit\n");

	if (token)
	{
		if (is_numeric(token->value))
		{
			exit_status = ft_atoi(token->value) % 256;
			token = token->next;
			if (token)
			{
				ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
				shell->exit_status = 1;
				return (1);
			}
		}
		else
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", STDERR_FILENO);
			exit_status = 255;
		}
	}

	cleanup(shell);
	exit(exit_status);
}
