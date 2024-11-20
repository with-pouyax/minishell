// src/internal_commands.c
#include "internal_commands.h"
#include "minishell.h"


int	execute_internal_commands(void)
{
	t_command	*cmd;
	t_token		*token;
	int			ret;

	cmd = g_data.commands;
	while (cmd)
	{
		token = cmd->token_list;
		while (token)
		{
			if (token->is_command && token->is_int)
			{
				if (ft_strcmp(token->value, "echo") == 0)
					ret = ft_echo(cmd);
				else if (ft_strcmp(token->value, "cd") == 0)
					ret = ft_cd(cmd);
				else if (ft_strcmp(token->value, "pwd") == 0)
					ret = ft_pwd(cmd);
				else if (ft_strcmp(token->value, "export") == 0)
					ret = ft_export(cmd);
				else if (ft_strcmp(token->value, "unset") == 0)
					ret = ft_unset(cmd);
				else if (ft_strcmp(token->value, "env") == 0)
					ret = ft_env(cmd);
				else if (ft_strcmp(token->value, "exit") == 0)
					ret = ft_exit_shell(cmd);
				else
				{
					// Not an internal command
					ft_putstr_fd("minishell: ", STDERR_FILENO);
					ft_putstr_fd(token->value, STDERR_FILENO);
					ft_putstr_fd(": command not found\n", STDERR_FILENO);
					g_data.exit_status = 127;
					return (-1); // Stop execution due to command not found
				}
				
				// Set global exit status based on command return value
				g_data.exit_status = ret;
				
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
