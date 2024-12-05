#include "../internal_commands.h"

void	print_exit_error(char *message, int *exit_status, int code)
{
	ft_putstr_fd(message, STDERR_FILENO);
	*exit_status = code;
}

/*
token = cmd->token_list->next  -->  Skip the command token
*/
int	ft_exit_shell(t_shell_data *shell, t_command *cmd)
{
	t_token	*token;
	int	exit_status;

	token = cmd->token_list->next;
	exit_status = shell->exit_status;
	//printf("exit\n");
	if (token)
	{
		if (is_numeric(token->value))
		{
			exit_status = ft_atoi(token->value) % 256;
			token = token->next;
			if (token)
			{
				print_exit_error("exit: too many arguments\n", &shell->exit_status, 1);
				return (1);
			}
		}
		else
			print_exit_error("exit:numeric arg required\n", &exit_status, 255);
	}
	cleanup(shell);
	rl_clear_history();
	exit(exit_status);
}
