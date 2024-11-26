#include "../minishell.h"

int **init_pipes(int cmds_nb)
{
    int **pipes;
    int i;

    i = 0;
    if (cmds_nb == 1)
        return (NULL);
    pipes = malloc(sizeof(int *) * (cmds_nb - 1));
    if (!pipes)
        exit (EXIT_FAILURE);
    while (i < (cmds_nb - 1))
    {
        pipes[i] = malloc(sizeof(int) * 2); //2 for read and write
        if (!pipes[i])
            exit (EXIT_FAILURE);
        if (pipe(pipes[i]) == -1)
            exit (EXIT_FAILURE);
        i++;
    }
    return (pipes);
}
void close_pipes_after_execution(t_shell_data *shell, int cmds_index)
{
    int **pipes = shell->pipes;
    int cmds_nb = shell->cmds_nb;

    // Close unused pipe file descriptors in parent process
    if (cmds_index != 0)  // Not the first command
        close(pipes[cmds_index - 1][0]);  // Close read end of previous pipe

    if (cmds_index != cmds_nb - 1)  // Not the last command
        close(pipes[cmds_index][1]);  // Close write end of current pipe
}

void close_all_pipes(int **pipes, int nb_cmds)
{
    int i;

    i = 0;
    while (i < nb_cmds - 1)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

void free_pipes(int **pipes, int nb_cmds)
{
    int i;

    i = 0;
    while (i < nb_cmds - 1)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        free(pipes[i]);
        i++;
    }
    free(pipes);
}
