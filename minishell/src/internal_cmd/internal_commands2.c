/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_commands2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 22:06:23 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:25:52 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_exit_child(t_shell_data *shell, t_command *cmd)
{
	int	exit_status;

	(void)cmd;
	exit_status = shell->exit_status;
	cleanup(shell);
	rl_clear_history();
	close(shell->saved_stdin);
	close(shell->saved_stdout);
	exit(exit_status);
}

void	free_pid_list(t_shell_data *shell)
{
	t_pid_node	*current;
	t_pid_node	*next;

	current = shell->pid_list;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	shell->pid_list = NULL;
}


int	fork_and_execute(t_shell_data *shell, t_command *cmds, t_token *token)
{
	pid_t	pid;

	signal(SIGPIPE, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		write_error("Fork failed", strerror(errno));
		return (-1);
	}
	else if (pid == 0)
	{
		close_all_pipes(shell->pipes, shell->cmds_nb);
		if (execute_command(shell, cmds, token, 0) == -1)
		{
			free_pid_list(shell);
			ft_exit_child(shell, cmds);
		}
		free_pid_list(shell);
		ft_exit_child(shell, cmds);
	}
	else
		store_pids(shell, pid);
	return (0);
}