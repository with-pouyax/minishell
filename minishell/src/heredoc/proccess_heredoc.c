/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proccess_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:57:36 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 00:58:26 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static void	process_heredoc_lines(t_shell_data *shell, \
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
