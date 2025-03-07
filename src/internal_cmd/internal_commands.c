/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 09:51:57 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:25:48 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_parent_command(t_shell_data *shell, t_command *cmds, \
t_token *token)
{
	if (!shell || !cmds || !token)
		return (-1);
	if (shell->cmds_nb > 1)
		return (1);
	return (execute_command(shell, cmds, token, 0));
}

int	is_parent_command(const char *cmd)
{
	return (ft_strcmp(cmd, "cd") == 0 || \
	ft_strcmp(cmd, "exit") == 0 || \
	ft_strcmp(cmd, "export") == 0 || \
	ft_strcmp(cmd, "unset") == 0);
}

/*
shell->exit_status = ret
----> Set global exit status based on command return value
*/

int	execute_command(t_shell_data *shell, t_command *cmd, \
t_token *token, int ret)
{
	if (!shell || !cmd || !token)
		return (-1);
	if (ft_strcmp(token->value, "echo") == 0)
		ret = ft_echo(shell, cmd);
	else if (ft_strcmp(token->value, "cd") == 0)
		ret = ft_cd(shell, cmd);
	else if (ft_strcmp(token->value, "pwd") == 0)
		ret = ft_pwd(shell, cmd);
	else if (ft_strcmp(token->value, "export") == 0)
		ret = ft_export(shell, cmd);
	else if (ft_strcmp(token->value, "unset") == 0)
		ret = ft_unset(shell, cmd);
	else if (ft_strcmp(token->value, "env") == 0)
		ret = ft_env(shell, cmd);
	else if (ft_strcmp(token->value, "exit") == 0)
		ret = ft_exit_shell(shell, cmd);
	else
	{
		write_error2(token->value, ": No such file or directory\n");
		shell->exit_status = 127;
		return (-1);
	}
	shell->exit_status = ret;
	return (ret);
}

int	execute_internal_commands(t_shell_data *shell, t_command *cmds)
{
	t_token	*token;

	if (!cmds || !cmds->token_list)
	{
		return (0);
	}
	token = cmds->token_list;
	while (token)
	{
		if (token->is_command)
		{
			if (is_parent_command(token->value))
			{
				if (execute_parent_command(shell, cmds, token) == -1)
					return (-1);
			}
			else
			{
				if (fork_and_execute(shell, cmds, token) == -1)
					return (-1);
			}
		}
		token = token->next;
	}
	return (0);
}
