/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirectiom_files.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:45:05 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:20:57 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to create directories in the path if they don't exist
#include <sys/stat.h>
#include <libgen.h>
#include <string.h>
#include <unistd.h>
/*
 If -2 then no file has been opened otherwise it means, another file is about
 to be opened and we need to close the previous one
 example: command < file1 < file2 > file3 > file4
   The last input redirection (< file2) will be the one that is actually used.
   The last output redirection (> file4) will be the one that is actually used.
*/
int	open_input_file(t_shell_data *shell, t_redirection *redir, int fd_in_prev)
{
	int	new_fd;

	if (fd_in_prev != -2)
		close(fd_in_prev);
	new_fd = open(redir->filename, O_RDONLY, 0);
	if (new_fd == -1)
	{
		if (!shell->last_error_file)
		{
			shell->last_error_file = redir->filename;
			shell->exit_status = -1;
		}
		return (-1);
	}
	else
	{
		dup2(new_fd, STDIN_FILENO);
		close(new_fd);
	}
	return (new_fd);
}

int	open_output_file(t_shell_data *shell, t_redirection *redir, int fd_out_prev)
{
	int	new_fd;

	if (fd_out_prev != -2)
		close(fd_out_prev);
	new_fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (new_fd == -1)
	{
		shell->last_error_file = redir->filename;
		shell->exit_status = -1;
		return (-1);
	}
	else
	{
		dup2(new_fd, STDOUT_FILENO);
		close(new_fd);
	}
	return (new_fd);
}

int	open_append_file(t_shell_data *shell, t_redirection *redir, int fd_out_prev)
{
	int	new_fd;

	if (fd_out_prev != -2)
		close(fd_out_prev);
	new_fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (new_fd == -1)
	{
		shell->last_error_file = redir->filename;
		shell->exit_status = -1;
		return (-1);
	}
	{
		dup2(new_fd, STDOUT_FILENO);
		close(new_fd);
	}
	return (new_fd);
}
