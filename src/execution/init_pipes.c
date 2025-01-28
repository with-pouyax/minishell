#include "../../include/minishell.h"

int	**init_pipes(t_shell_data *shell, int cmds_nb)
{
	int	**pipes;
	int	i;

	i = 0;
	if (cmds_nb == 1)
		return (NULL);
	pipes = NULL; //malloc(sizeof(int *) * (cmds_nb - 1));
	if (!pipes)
	{
		ft_putstr_fd("malloc failed\n", 2);
		ft_exit_child(shell);
	}	
	while (i < (cmds_nb - 1))
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
			return (NULL);
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe error");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (pipes);
}

void	close_pipes_after_execution(t_shell_data *shell, int cmds_index)
{
	int	**pipes;
	int	cmds_nb;

	pipes = shell->pipes;
	cmds_nb = shell->cmds_nb;
	if (cmds_index != 0)
		close(pipes[cmds_index - 1][0]);
	if (cmds_index != cmds_nb - 1)
		close(pipes[cmds_index][1]);
}

void	close_all_pipes(int **pipes, int nb_cmds)
{
	int	i;

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
