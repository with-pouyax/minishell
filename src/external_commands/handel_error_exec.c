/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_error_exec.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:59:12 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/24 14:20:11 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void	handle_exec_error(t_shell_data *shell, char *cmd, char *message,
// 			int exit_code)
// {
// 	ft_putstr_fd(cmd, STDERR_FILENO);
// 	ft_putstr_fd(": ", STDERR_FILENO);
// 	ft_putendl_fd(message, STDERR_FILENO);
// }
void	write_error(char *exec_name)
{
	ft_putstr_fd(exec_name, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	write_error2(char *exec_name, char *err_message)
{
	ft_putstr_fd(exec_name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(err_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	get_exec_error_code(int err)
{
	if (err == ENOENT)
		return (127);
	if (err == EACCES)
		return (126);
	return (1);
}

void	free_paths(char **paths)
{
	int	i;

	if (!paths)
		return ;
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}
