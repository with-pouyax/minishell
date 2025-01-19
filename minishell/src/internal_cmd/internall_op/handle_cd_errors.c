/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cd_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 09:44:27 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 09:44:48 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	handle_chdir_error(t_shell_data *shell, char *expanded_path)
{
	perror("minishell: cd");
	free(expanded_path);
	shell->exit_status = 1;
	return (1);
}

int	handle_memory_error(t_shell_data *shell)
{
	ft_putstr_fd("minishell: cd: memory allocation error\n", STDERR_FILENO);
	shell->exit_status = 1;
	return (1);
}

int	handle_no_home(t_shell_data *shell)
{
	ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
	shell->exit_status = 1;
	return (1);
}
