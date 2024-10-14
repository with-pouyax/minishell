#include "minishell.h"

int handle_heredoc(char *delimiter, t_command *cmd)
{
    int     fd[2];
    char    *line;

    if (pipe(fd) < 0)
    {
        perror("minishell: pipe");
        return (1);
    }
    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        ft_putendl_fd(line, fd[1]);
        free(line);
    }
    close(fd[1]);
    if (cmd->input_fd != STDIN_FILENO)
        close(cmd->input_fd);
    cmd->input_fd = fd[0];
    return (0);
}
