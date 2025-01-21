/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:36:46 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/21 15:37:30 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :  handle syntax errors
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//     🏷  error_message -> error message to print
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- if there is an error message we print it to the standard error.
//        a- we free the input and full_input.
//        b- we set the input and full_input to NULL.
//        c- we set the exit status to 2.
//     2- if everything is fine we return 0.
/******************************************************************************/

int	check_syntax_error(t_shell_data *shell, char *error_message)
{
	if (error_message)
	{
		ft_putstr_fd(error_message, STDERR_FILENO);
		free(shell->input);
		free(shell->full_input);
		shell->input = NULL;
		shell->full_input = NULL;
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}
