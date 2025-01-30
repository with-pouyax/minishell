/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:46:18 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:21:34 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

/*
We need to close all the pipes in the child process because: we already
changed the stdout and stdin inside set_redirs and set_pipes()

 If execve call is successful, the following lines are never executed
 chon : the child process will kill itself when finished
*/
void	exec_external_child(t_shell_data *shell, char *cmd_path, char **argv)
{
	int exit_status;

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

void    if_convert_fail(t_shell_data *shell, char	**arr_token)
{
	free(arr_token);
	close_all_pipes(shell->pipes, shell->cmds_nb);
	ft_exit_child(shell);
}

void    if_fork_fail(char	*cmd_path, char	**arr_token, int token_count)
{
	write_error("Fork failed");
	free(cmd_path);
	free_argv(arr_token, token_count);
	return  ;
}

void	if_arr_token_fail(t_shell_data *shell)
{
	write_error("Memory allocation failed");
	ft_exit_child(shell);
}
void	handle_malloc_fail(t_shell_data *shell)
{
	write_error("Memory allocation failed");
	ft_exit_child(shell);
}
