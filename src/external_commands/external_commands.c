/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:46:18 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/30 15:29:54 by pghajard         ###   ########.fr       */
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

char	*check_cmd_path_null1(t_shell_data *shell, t_command *cmds,
				char **arr_token, char *cmd_path)
{
	if (!cmd_path)
	{
		write_error2(arr_token[0], "command not found");
		shell->exit_status = 127;
		free_argv(arr_token, token_list_length(cmds->token_list));
		return (NULL);
	}
	return (cmd_path);
}

char	*check_dot_cmd_path1(t_shell_data *shell, t_command *cmds,
				char **arr_token, char *cmd_path)
{
	if (cmd_path[0] == '.' && (cmd_path[1] == '\0'
			|| (cmd_path[1] == '.' && cmd_path[2] == '\0')))
	{
		write_error2(cmd_path, "command not found");
		shell->exit_status = 127;
		free(cmd_path);
		free_argv(arr_token, token_list_length(cmds->token_list));
		return (NULL);
	}
	return (cmd_path);
}

char	*check_dir_cmd_path1(t_shell_data *shell, t_command *cmds,
				char **arr_token, char *cmd_path)
{
	struct stat	path_stat;

	if (stat(cmd_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		write_error2(cmd_path, "Is a directory");
		shell->exit_status = 126;
		free(cmd_path);
		free_argv(arr_token, token_list_length(cmds->token_list));
		return (NULL);
	}
	return (cmd_path);
}
