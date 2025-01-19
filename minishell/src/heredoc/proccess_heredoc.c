/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proccess_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:57:36 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 20:16:18 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  line -> the line we want to handle
//     🏷  redir -> the redirection we want to prepare
//     🏷  fd -> the file descriptor
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the signal status is not 0, meaning we received a signial
//        a- we free the line and return 1.
//     2- if the line is equal to the delimiter
//        a- we free the line and return 1.
//     3- if the delimiter is not quoted
//        a- using expand_and_write_line() we expand and write the line.
//           I- if there is an error we free the line and return 1.
//     4- if the delimiter is quoted
//        a- we write the line to the file descriptor.
//        b- we free the line to the file descriptor.
//     5- we free the line and return 0.
//
/******************************************************************************/

int	handle_heredoc_line(t_shell_data *shell, char *line, \
t_redirection *redir, int fd)
{
	if (g_signal.signal_status)
		return (free(line), 1);
	if (ft_strcmp(line, redir->delimiter) == 0)
		return (free(line), 1);
	if (!redir->delimiter_quoted)
	{
		if (expand_and_write_line(shell, line, fd))
			return (free(line), 1);
	}
	else
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
	free(line);
	return (0);
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  redir         ->
//     🏷  tmp_filename  ->
//     🏷  fd            ->
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using check_delimiter_quotes() we check if the delimiter is quoted.
//        a- if the delimiter is less than 0, it means there is an error and we
//           return 1.
//     2- using generate_temp_filename() we generate a temporary filename and
//        store it in tmp_filename.
//        a- if there is an error we return 1.
//     3- using open() we open the tmp_filename with the proper flags and store
//        the file descriptor in fd.
//        - O_CREAT: create the file if it does not exist.
//        - O_WRONLY: open the file for writing.
//        - O_TRUNC: truncate the file to 0 length, meaning it will delete the
//          content of the file if it exists and open it for writing only.
//        - 0600: the file permission means that the owner can read and write
//          the file and no one else can access it in any way.
//        a- if there is an error opening we call heredoc_open_error() and
//        return its return, which is 1.
//     4- at the end we return 0.
//
/******************************************************************************/

int	initialize_heredoc(t_redirection *redir, char **tmp_filename, int *fd)
{
	redir->delimiter_quoted = check_delimiter_quotes(redir);
	if (redir->delimiter_quoted < 0)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (1);
	}
	*tmp_filename = generate_temp_filename();
	if (!*tmp_filename)
		return (1);
	*fd = open(*tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (*fd < 0)
		return (heredoc_open_error(*tmp_filename));
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  redir -> the redirection we want to prepare
//     🏷  fd    -> the file descriptor
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- we se shell->in_heredoc to 1 to indicate that we are in a heredoc.
//     2- we setup the signal handlers with type 1 to handle the signals in
//        the heredoc.
//     3- we stat an infinite loop.
//     4- we wait for the user to enter an input and store it in line.
//        a- if user press ctrl + D we print a warning message and break the
//           loop.
//     5- using handle_heredoc_line() we handle the heredoc line.
//        a- if there is an error or the user entered the delimiter we break
//           the loop.
//     6- now that we are out of the loop we close the file descriptor.
//     7- we set shell->in_heredoc to 0 to indicate that we are out of the
//        heredoc.
//
/******************************************************************************/

void	process_heredoc_lines(t_shell_data *shell, \
t_redirection *redir, int fd)
{
	char	*line;

	shell->in_heredoc = 1;
	setup_signal_handlers(1);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("minishell: warning: here-document delimited by end-of-file\
			 (wanted '%s')\n", shell->filename_or_delimiter);
			break ;
		}
		if (handle_heredoc_line(shell, line, redir, fd))
		{
			break ;
		}
	}
	close(fd);
	shell->in_heredoc = 0;
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  redir -> the redirection we want to prepare
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using initialize_heredoc() we initialize the heredoc redirection.
//        a- if there is an error we return 1.
//     2- using process_heredoc_lines() we process the heredoc lines.
//     3- we set the heredoc_file to the tmp_filename.
//     4- we return 0.
//
/******************************************************************************/

int	read_heredoc_content(t_shell_data *shell, t_redirection *redir)
{
	char		*tmp_filename;
	int			fd;

	if (initialize_heredoc(redir, &tmp_filename, &fd))
		return (1);
	process_heredoc_lines(shell, redir, fd);
	redir->heredoc_file = tmp_filename;
	return (0);
}
