#include "../../include/minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourname <yourname@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:00:00 by yourname          #+#    #+#             */
/*   Updated: 2025/01/30 10:00:00 by yourname         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


static int	**allocate_pipes1(t_shell_data *shell, int cmds_nb, int *ret)
{
	int	**pipes;

	(void)shell;
	if (cmds_nb == 1)
		return (NULL);
	pipes = malloc(sizeof(int *) * (cmds_nb - 1));
	if (!pipes)
	{
		*ret = 1;
		ft_putstr_fd("malloc failed\n", 2);
		return (NULL);
	}
	return (pipes);
}

static int	create_each_pipe1(t_shell_data *shell, int **pipes,
			int cmds_nb, int *ret)
{
	int	i;

	i = 0;
	while (i < (cmds_nb - 1))
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			free(pipes);
			*ret = 1;
			ft_clean(shell);
			return (-1);
		}
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe error");
			*ret = 1;
			ft_clean(shell);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	**init_pipes(t_shell_data *shell, int cmds_nb, int *ret)
{
	int	**pipes;

	pipes = allocate_pipes1(shell, cmds_nb, ret);
	if (!pipes)
		return (NULL);
	if (create_each_pipe1(shell, pipes, cmds_nb, ret) == -1)
		return (NULL);
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
