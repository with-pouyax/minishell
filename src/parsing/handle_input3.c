/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 23:00:19 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:30:42 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/*****************************************************************************/
//                        🚀 handle_allocation 🚀
/*****************************************************************************/
// 🎯 Purpose  :  allocate resources needed for the shell
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we allocate memory dynamically in heap for shell->full_input.
//     2- if there is an error allocating the resources we print an error
//        message and return 1.
//     3- if everything is fine we return 0.
/******************************************************************************/

int	handle_allocation(t_shell_data *shell)
{
	shell->full_input = ft_strdup(shell->input);
	if (!shell->full_input)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

/*****************************************************************************/
//                            🚀 read_input 🚀
/*****************************************************************************/
// 🎯 Purpose  :    read input from the user
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we read the input using readline() and store the return value in
//        shell->input, we are prompted with PROMPT.
//     2- if shell->input is NULL (means user pressed ctrl+d) we call
//        handle_ctrl_d()  to handle the ctrl+d signal and return 1.
//     3- if everything is fine we return 0.
/******************************************************************************/
int	read_input(t_shell_data *shell)
{
	shell->input = readline(PROMPT);
	if (!shell->input)
	{
		handle_ctrl_d(shell);
		return (1);
	}
	return (0);
}
/*****************************************************************************/
//                        🚀 validate_input_length 🚀
/*****************************************************************************/
// 🎯 Purpose  :  allocate resources needed for the shell
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- if there is an error allocating the resources we return 1.
//     2- if everything is fine we return 0.
/******************************************************************************/

int	allocate_resources(t_shell_data *shell)
{
	if (handle_allocation(shell))
	{
		return (1);
	}
	return (0);
}

/*****************************************************************************/
//                        🚀 validate_input_length 🚀
/*****************************************************************************/
// 🎯 Purpose  :  check if the input is not too long
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- using check_input_length() we check if the input is not too long.
//        a- if it is too long we call cleanup() and cleanup the all the
//           resources and return 1.
//	 2- if everything is fine we return 0.
/******************************************************************************/

int	validate_input_length(t_shell_data *shell)
{
	if (check_input_length(shell))
	{
		cleanup(shell);
		return (1);
	}
	return (0);
}
/*****************************************************************************/
//                        🚀 handle_empty_input 🚀
/*****************************************************************************/
// 🎯 Purpose  :  check if the function is empty
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- we free the shell->input and set it to NULL.
//     2- we set the signal status to 0 because we go back to the main process.
/******************************************************************************/

void	handle_empty_input(t_shell_data *shell)
{
	free(shell->input);
	shell->input = NULL;
	g_signal.signal_status = 0;
}
