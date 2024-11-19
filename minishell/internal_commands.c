// src/internal_commands.c
#include "internal_commands.h"

int	execute_internal_commands(t_shell_data *shell)
{
	t_command	*cmd;
	t_token		*token;
	int			ret;

	cmd = shell->commands;
	while (cmd)
	{
		token = cmd->token_list;
		while (token)
		{
			if (token->is_command && token->is_int)
			{
				if (ft_strcmp(token->value, "echo") == 0)
					ret = ft_echo(shell, cmd);
				else if (ft_strcmp(token->value, "cd") == 0)
					ret = ft_cd(shell, cmd);
				else if (ft_strcmp(token->value, "pwd") == 0)
					ret = ft_pwd(shell, cmd);
				else if (ft_strcmp(token->value, "export") == 0)
					ret = ft_export(shell, cmd); // last_one
				else if (ft_strcmp(token->value, "unset") == 0)
					ret = ft_unset(shell, cmd);
				else if (ft_strcmp(token->value, "env") == 0)
					ret = ft_env(shell, cmd);
				else if (ft_strcmp(token->value, "exit") == 0)
					ret = ft_exit_shell(shell, cmd);
				else
				{
					// Not an internal command
					ft_putstr_fd("minishell: ", STDERR_FILENO);
					ft_putstr_fd(token->value, STDERR_FILENO);
					ft_putstr_fd(": command not found\n", STDERR_FILENO);
					shell->exit_status = 127;
					return (-1); // Stop execution due to command not found
				}
				
				// Set global exit status based on command return value
				shell->exit_status = ret;
				
				// Stop further processing if an error occurred
				if (ret != 0)
					return (-1);

				break ; // Move to the next command after handling
			}
			token = token->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
