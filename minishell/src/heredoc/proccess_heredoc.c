#include "../minishell.h"

int handle_heredoc_line(t_shell_data *shell, char *line, t_redirection *redir, int fd)
{
    if (g_signal.signal_status)
    {
        free(line);
        return (1);
    }
    if (ft_strcmp(line, redir->delimiter) == 0)
    {
        free(line);
        return (1);
    }
    if (!redir->delimiter_quoted)
    {
        if (expand_and_write_line(shell, line, fd))
        {
            free(line);
            return (1);
        }
    }
    else
    {
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
    }
    free(line);
    return (0);
}


int read_heredoc_content(t_shell_data *shell, t_redirection *redir)
{
    char *line;
    int fd;
    char *tmp_filename;

    redir->delimiter_quoted = check_delimiter_quotes(redir); // Adjusted to use redirection
    if (redir->delimiter_quoted < 0)
    {
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
        return (1);
    }
    tmp_filename = generate_temp_filename(); // [x] 
    if (!tmp_filename)
        return (1);
    fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600); // [x]
    if (fd < 0)
        return (heredoc_open_error(tmp_filename));
    setup_signal_handlers(1);
    shell->in_heredoc = 1;
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            printf("minishell: warning: here-document delimited by end-of-file (wanted '%s')\n", shell->filename_or_delimiter);
            break;
        }
        if (!line || handle_heredoc_line(shell, line, redir, fd))
        {
            setup_signal_handlers(1);
            break;
        }
    }
    close(fd);
    shell->in_heredoc = 0;
    redir->heredoc_file = tmp_filename; // Store the filename in redirection
    return (0);
}
