/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:46:18 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/30 15:29:54 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*resolve_command_path(t_shell_data *shell, t_command *cmds,
			char **arr_token)
{
	char	*cmd_path;

	cmd_path = get_command_path(shell, cmds->token_list);
	cmd_path = check_cmd_path_null1(shell, cmds, arr_token, cmd_path);
	if (!cmd_path)
		return (NULL);
	cmd_path = check_dot_cmd_path1(shell, cmds, arr_token, cmd_path);
	if (!cmd_path)
		return (NULL);
	cmd_path = check_dir_cmd_path1(shell, cmds, arr_token, cmd_path);
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

char	**allocate_and_fill_argv1(t_shell_data *shell, t_command *cmds,
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
void	fork_and_exec1(t_shell_data *shell, char *cmd_path,
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
