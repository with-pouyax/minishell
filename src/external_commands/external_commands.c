/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:46:18 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/30 15:01:23 by pghajard         ###   ########.fr       */
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

char	*resolve_command_path(t_shell_data *shell, t_command *cmds,
				char **arr_token)
{
	char	*cmd_path;
	struct stat path_stat;

	cmd_path = get_command_path(shell, cmds->token_list);
	if (!cmd_path)
	{
		write_error2(arr_token[0], "command not found");
		shell->exit_status = 127;
		free_argv(arr_token, token_list_length(cmds->token_list));
		return (NULL);
	}
	if (cmd_path[0] == '.' && (cmd_path[1] == '\0' || (cmd_path[1] == '.' && cmd_path[2] == '\0')))
	{
		write_error2(cmd_path, "command not found");
		shell->exit_status = 127;
		free(cmd_path);
		free_argv(arr_token, token_list_length(cmds->token_list));
		return (NULL);
	}
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

/*
We need to close all the pipes in the child process because: we already
changed the stdout and stdin inside set_redirs and set_pipes()

 If execve call is successful, the following lines are never executed
 chon : the child process will kill itself when finished
*/
void	exec_external_child(t_shell_data *shell, char *cmd_path, char **argv)
{
	int	exit_status;

	close_all_pipes(shell->pipes, shell->cmds_nb);
	exit_status = execve(cmd_path, argv, shell->envp);
	if (exit_status == -1)
	{
		free(cmd_path);
		write_error(argv[0]);
		free(argv);
		argv = NULL;
	}
}






static char	**allocate_and_fill_argv1(t_shell_data *shell, t_command *cmds,
		int *token_count)
{
	char	**arr_token;

	*token_count = token_list_length(cmds->token_list);
	arr_token = malloc(sizeof(char *) * (*token_count + 1));
	if (!arr_token)
	{
		write_error("Memory allocation failed");
		ft_exit_child(shell);
	}
	if (convert_tokens_to_argv(cmds->token_list, arr_token) == -1)
	{
		free(arr_token);
		close_all_pipes(shell->pipes, shell->cmds_nb);
		ft_exit_child(shell);
	}
	return (arr_token);
}

/*
** Forks and executes the child process or stores the PID in parent.
** Frees resources and returns on fork failure.
*/
static void	fork_and_exec1(t_shell_data *shell, char *cmd_path,
		char **arr_token, int token_count)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		write_error("Fork failed");
		free(cmd_path);
		free_argv(arr_token, token_count);
		return ;
	}
	else if (pid == 0)
	{
		exec_external_child(shell, cmd_path, arr_token);
		ft_exit_child(shell);
	}
	else
		store_pids(shell, pid);
}

/*
** Main function that manages execution of external commands.
** Splits into smaller functions to respect Norminette line limits.
*/
void	execute_external_commands(t_shell_data *shell, t_command *cmds)
{
	int		token_count;
	char	*cmd_path;
	char	**arr_token;

	arr_token = allocate_and_fill_argv1(shell, cmds, &token_count);
	if (!arr_token[0])
	{
		free(arr_token);
		return ;
	}
	cmd_path = resolve_command_path(shell, cmds, arr_token);
	if (!cmd_path)
		return ;
	fork_and_exec1(shell, cmd_path, arr_token, token_count);
	free(cmd_path);
	free_argv(arr_token, token_count);
}



void	forking(t_shell_data *shell, t_command *cmds)
{
	setup_signal_handlers(2);
	if (cmds->token_list && cmds->token_list->is_int)
	{
		execute_internal_commands(shell, cmds);
	}
	else
	{
		execute_external_commands(shell, cmds);
	}
}
