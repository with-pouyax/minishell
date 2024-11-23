#include "../minishell.h"

int handle_heredoc_line(t_shell_data *shell, char *line, t_redirection *redir, int fd, int delimiter_quoted)
{
    if (ft_strcmp(line, redir->delimiter) == 0)
    {
        free(line);
        return (1);
    }
    if (!delimiter_quoted)
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
    int delimiter_quoted;

    delimiter_quoted = check_delimiter_quotes(redir); // Adjusted to use redirection
    tmp_filename = generate_temp_filename();
    if (!tmp_filename)
        return (1);
    fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd < 0)
        return (heredoc_open_error(tmp_filename));
    while (1)
    {
        line = readline("> ");
        if (!line || handle_heredoc_line(shell, line, redir, fd, delimiter_quoted))
            break;
    }
    close(fd);
    redir->heredoc_file = tmp_filename; // Store the filename in redirection
    return (0);
}


void process_heredocs(t_shell_data *shell)
{
    t_command *cmd;
    t_redirection *redir;

    cmd = shell->commands;
    while (cmd) // Loop through all commands
    {
        redir = cmd->redirections;
        while (redir) // Loop through all redirections in the command
        {
            if (redir->type == REDIR_HEREDOC) // If the redirection is a heredoc
            {
                if (read_heredoc_content(shell, redir)) // Read the heredoc content
                {
                    shell->error_flag = 1;
                    return;
                }
            }
            redir = redir->next;
        }
        cmd = cmd->next;
    }
}
