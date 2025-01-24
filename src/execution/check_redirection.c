/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:43:38 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:20:47 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	check_input(t_shell_data *shell, t_redirection *redir, int cmds_index)
{
	if (has_redirs(redir, REDIR_INPUT))
		return ;
	if (cmds_index != 0)
	{
		if (dup2(shell->pipes[cmds_index - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2 failed for input pipe");
			shell->exit_status = EXIT_FAILURE;
			return ;
		}
	}
}

void	check_output(t_shell_data *shell, t_redirection *redir, int cmds_index)
{
	(void)redir;
	if (has_redirs(redir, REDIR_OUTPUT) || has_redirs(redir, REDIR_APPEND))
		return ;
	if (cmds_index != shell->cmds_nb - 1)
	{
		if (dup2(shell->pipes[cmds_index][1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed for input pipe");
			shell->exit_status = EXIT_FAILURE;
			return ;
		}
	}
}

/*
 in open_all_files(redirs) processes:
if multiple input redirections exist, the last file is the one that
remains open because the file descriptor is overwritten.
cmd < input1 < input2 will read input only from input2.
cmd > output1 > output2 will write output only to output2.
The first process:
        Doesn't set up piping for STDIN since it has no previous process.
The middle processes:
        Use the pipe from the previous process for STDIN.
        Write their STDOUT into their own pipe for the next process to read.
The last process:
        Doesn't set up piping for STDOUT since there's no next process
       	to send data
diagram for a pipeline with three processes (A | B | C):
Process A:
    Reads input normally (STDIN).
    (when it is first cmd so not gonna do anything in check_input_redir();)
    Writes output to pipe[0][1].
Process B:
    Reads input from pipe[0][0].
    Writes output to pipe[1][1].
Process C:
    Reads input from pipe[1][0].
    Writes output normally (STDOUT).
    (when it is last cmd so not gonna do anything in check_output_redir();)
*/

void	set_redirection(t_shell_data *shell, t_redirection *redir)
{
	int	exit_code;

	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			handle_heredoc(shell, redir);
			if (shell->exit_status == EXIT_FAILURE)
				return ;
		}
		else
		{
			exit_code = open_all_files(shell, redir);
			if (exit_code == EXIT_FAILURE)
				return ;
		}
		redir = redir->next;
	}
	shell->exit_status = EXIT_SUCCESS;
}

void	set_pipes(t_shell_data *shell, t_redirection *redir, int cmds_index)
{
	check_input(shell, redir, cmds_index);
	check_output(shell, redir, cmds_index);
}
