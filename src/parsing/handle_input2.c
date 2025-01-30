/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 22:59:25 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:30:38 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*****************************************************************************/
//                            🚀 handle_ctrl_d 🚀
/*****************************************************************************/
// 🎯 Purpose  :    handle control + d
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- we call cleanup() to free all the resources and exit with the proper
//        exit status.
/******************************************************************************/

void	handle_ctrl_d(t_shell_data *shell)
{
	cleanup(shell);
	exit(shell->exit_status);
}
/*****************************************************************************/
// 🎯 Purpose  :  purpose: process and execute the commands
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we preprocess (meaning we tokenize and parse) the input using
//        preprocess_input().
//        a- if there is an error in the preprocess_input() we return 1.
//     2- if there are commands in the shell we execute them using execution().
//     3- if there are no commands (usually because of syntax errors or empty
//        input) we set the exit status to 2.
//     4- after we are passing and executing the commands we free the shell
//        resources using free_shell_resources().
/******************************************************************************/

int	process_and_execute_commands(t_shell_data *shell)
{
	if (preprocess_input(shell) != 0)
		return (1);
	if (shell->commands)
	{
		execution(shell);
	}
	else
		shell->exit_status = 2;
	free_shell_resources(shell);
	return (0);
}

/*****************************************************************************/
//                      🚀 check_and_handle_syntax_errors 🚀
/*****************************************************************************/
// 🎯 Purpose  :  Check for some syntax errors
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- using check_unclosed_quotes() we check if there are any unclosed
//        quotes.
//        a- if there are unclosed quotes we print an error message and
//           cleanup the shell and return 1.
//     2- using check_leading_pipe() we check if there is pipe at the beginning
//        of the input.
//        a- if there is a pipe at the beginning we print an error message and
//           cleanup the shell and return 1.
//     3- using check_trailing_pipe() we check if there is a pipe at the end of
//        the input.
//        a- if there is a pipe at the end we print an error message and
//           cleanup the shell and return 1.
//     4- if everything is fine we return 0.
/******************************************************************************/

int	check_and_handle_syntax_errors(t_shell_data *shell)
{
	if (check_unclosed_quotes(shell->input))
	{
		check_syntax_error(shell, "minishell: syntax error: unclosed quotes\n");
		cleanup(shell);
		return (1);
	}
	else if (check_leading_pipe(shell->input))
	{
		check_syntax_error(shell, "syntax error near unexpected token `|'\n");
		cleanup(shell);
		return (1);
	}
	else if (check_trailing_pipe(shell->input))
	{
		check_syntax_error(shell, "minishell: syntax error near unexpected \
		token `|'\n");
		cleanup(shell);
		return (1);
	}
	return (0);
}

/*****************************************************************************/
//                        🚀 check_input_length 🚀
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
//     1- if the input is longer than MAX_INPUT_LENGTH we print an error
//		message and free the input and return 1.
//     2- if everything is fine we return 0.
/******************************************************************************/

int	check_input_length(t_shell_data *shell)
{
	if (ft_strlen(shell->input) > MAX_INPUT_LENGTH)
	{
		ft_putstr_fd("minishell: input too long\n", STDERR_FILENO);
		free(shell->input);
		return (1);
	}
	return (0);
}

/*****************************************************************************/
//                        🚀 add_to_history_if_needed 🚀
/*****************************************************************************/
// 🎯 Purpose  :  add to history if input is not empty
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the input is not empty we add it to the history.
/******************************************************************************/

void	add_to_history_if_needed(t_shell_data *shell)
{
	if (ft_strlen(shell->full_input) > 0)
		add_history(shell->full_input);
}
