#include "../minishell.h"

int handle_heredoc_line(t_shell_data *shell, char *line, t_redirection *redir, int fd)
{
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
    tmp_filename = generate_temp_filename();
    if (!tmp_filename)
        return (1);
    fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd < 0)
        return (heredoc_open_error(tmp_filename));
    while (1)
    {
        line = readline("> ");
        if (!line || handle_heredoc_line(shell, line, redir, fd))
            break;
    }
    close(fd);
    redir->heredoc_file = tmp_filename; // Store the filename in redirection
    return (0);
}
