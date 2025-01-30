/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:46:18 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:21:34 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_argv(char **argv, int count)
{
	int	i;

	i = 0;
	if (!argv)
		return ;
	while (i < count)
	{
		if (argv[i])
		{
			free(argv[i]);
			argv[i] = NULL;
		}
		i++;
	}
	free(argv);
}

/*
Flow of Execution
Parent Process:

Forks a child.
Tracks the child's PID for cleanup later.
Child Process:

Closes unused pipes.
Executes the command using execve().
On failure, exits with the appropriate error code.
Post-Execution:

After all commands in a pipeline are forked, the parent waits
for all child processes using waitpid().
*/

int	token_list_length(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		count++;
		token = token->next;
	}
	return (count);
}

char	*handle_invalid_command(char *cmd_path, t_shell_data *shell, 
				char **arr_token, int status)
{
	if (status == 127)
		write_error2(cmd_path, "command not found");
	else
		write_error2(cmd_path, "Is a directory");
	shell->exit_status = status;
	free(cmd_path);
	free_argv(arr_token, 0);
	return (NULL);
}

char	*resolve_command_path(t_shell_data *shell,
		t_command *cmds, char **arr_token)
{
	char		*cmd_path;
	struct stat	path_stat;

	cmd_path = get_command_path(shell, cmds->token_list);
	if (!cmd_path)
		return (handle_invalid_command(arr_token[0], shell, arr_token, 127));
	if (cmd_path[0] == '.')
	{
		if (!cmd_path[1] || (cmd_path[1] == '.' && !cmd_path[2]))
			return (handle_invalid_command(cmd_path, shell, arr_token, 127));
	}
	if (stat(cmd_path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
			return (handle_invalid_command(cmd_path, shell, arr_token, 126));
	}
	return (cmd_path);
}



int	convert_tokens_to_argv(t_token *token_list, char **argv)
{
	int		i;
	t_token	*temp;

	i = 0;
	temp = token_list;
	while (temp)
	{
		argv[i] = ft_strdup(temp->value);
		if (!argv[i])
		{
			while (i > 0)
				free(argv[--i]);
			return (-1);
		}
		temp = temp->next;
		i++;
	}
	argv[i] = NULL;
	return (0);
}
