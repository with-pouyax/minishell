/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:45:05 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:20:57 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	has_redirs(t_redirection *redir, t_redirection_type type )
{
	while (redir)
	{
		if (redir->type == type)
			return (1);
		redir = redir->next;
	}
	return (0);
}

static int	handle_redir(t_shell_data *shell, t_redirection *redir,
				int *fd_input, int *fd_output)
{
	if (redir->type == REDIR_INPUT)
		*fd_input = open_input_file(shell, redir, *fd_input);
	else if (redir->type == REDIR_OUTPUT)
		*fd_output = open_output_file(shell, redir, *fd_output);
	else if (redir->type == REDIR_APPEND)
		*fd_output = open_append_file(shell, redir, *fd_output);
	if (*fd_input == -1 || *fd_output == -1)
	{
		write_error2(shell->last_error_file, "No such file or directory");
		shell->exit_status = 1;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	finalize_redir(t_shell_data *shell, int fd_input, int fd_output)
{
	if (shell->exit_status != 0 && shell->last_error_file)
		write_error(shell->last_error_file);
	if (fd_input == -1 || fd_output == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

// use of fds here : to check If any of the open_file() returned -1,
// it means there was an error
// Opens all files. Only the last of its type is left open, others are closed.
int	open_all_files(t_shell_data *shell, t_redirection *redir)
{
	int	fd_input;
	int	fd_output;

	fd_input = -2;
	fd_output = -2;
	shell->last_error_file = NULL;
	while (redir)
	{
		if (handle_redir(shell, redir, &fd_input, &fd_output) == EXIT_FAILURE)
		{
			shell->exit_status = EXIT_FAILURE;
			return (EXIT_FAILURE);
		}
		redir = redir->next;
	}
	return (finalize_redir(shell, fd_input, fd_output));
}
