/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 09:43:16 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 09:43:27 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//     🏷  cmd -> our command structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- using getcwd() we get the current working directory.
//     2- if there it was successful we print the current working directory
//        and set the exit status to 0.
//     3- if there was an error we print an error message and set the exit
//        status to 1.
//
//
/******************************************************************************/

int	ft_pwd(t_shell_data *shell, t_command *cmd)
{
	char	cwd[PATH_MAX];

	(void)cmd;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		shell->exit_status = 0;
	}
	else
	{
		perror("minishell: pwd");
		shell->exit_status = 1;
	}
	return (0);
}
