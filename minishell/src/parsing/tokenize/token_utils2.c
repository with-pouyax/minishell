/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:31:36 by pghajard          #+#    #+#             */
/*   Updated: 2025/01/22 13:33:08 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :  handle pipe errors
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  new_redir -> the redirection we want to prepare
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- if type of new_redir is REDIR_HEREDOC we call
//     handle_heredoc_redirection()
//        a- if there is an error we return 1.
//     2- if the type of new_redir is not REDIR_HEREDOC we set the filename
//        to the shell filename_or_delimiter.
//
/******************************************************************************/

int	finalize_redirection(t_shell_data *shell, t_redirection *new_redir)
{
	if (new_redir->type == REDIR_HEREDOC)
	{
		if (handle_heredoc_redirection(shell, new_redir, \
		shell->filename_or_delimiter))
			return (1);
	}
	else
		new_redir->filename = shell->filename_or_delimiter;
	shell->filename_or_delimiter = NULL;
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :  handle pipe errors
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  input -> user input
//     🏷  i -> index of the current character in the input
//     🏷  new_redir -> the redirection we want to prepare
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the next character in the input is |, >, <, we call
//        handle_syntax_error_s() to handle the error.
//     2- if none of the above,  still there is an error and we call
//        handle_syntax_error_s() to handle the error.
//
/******************************************************************************/

int	handle_pipe_op(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir)
{
	if (input[*i + 1] == '|')
		return (handle_syntax_error_s(shell, new_redir, "||"));
	else if (input[*i + 1] == '>')
		return (handle_syntax_error_s(shell, new_redir, ">|"));
	else if (input[*i + 1] == '<')
		return (handle_syntax_error_s(shell, new_redir, "<|"));
	else if (input[*i + 1] == '\0' || input[*i + 1] == ' ' || \
	input[*i + 1] == '\n')
		return (handle_syntax_error_s(shell, new_redir, "|"));
	else
		return (handle_syntax_error_s(shell, new_redir, "|"));
}

/*****************************************************************************/
// 💡 Notes:
//     1- no explanation needed.
//
/******************************************************************************/

int	handle_greater_operator(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir)
{
	if (input[*i + 1] == '>')
		return (handle_syntax_error_s(shell, new_redir, ">>"));
	else
		return (handle_syntax_error_s(shell, new_redir, ">"));
}
/*****************************************************************************/
// 💡 Notes:
//     1- no explanation needed.
//
/******************************************************************************/

int	handle_less_operator(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir)
{
	if (input[*i + 1] == '<')
		return (handle_syntax_error_s(shell, new_redir, "<<"));
	else
		return (handle_syntax_error_s(shell, new_redir, "<"));
}

/*****************************************************************************/
// 🎯 Purpose  :  check operators errors
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  input -> user input
//     🏷  i -> index of the current character in the input
//     🏷  new_redir -> the redirection we want to prepare
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the current character in the input is | we call handle_pipe_op()
//        to handle the pipe operator error.
//     2- if the current character in the input is > we call
//     handle_greater_operator() to handle the greater operator error.
//     3- if the current character in the input is < we call
//     handle_less_operator() to handle the less operator error.
//     4- if none of the above,  means there is no operator error and we
//     return 0.
//
/******************************************************************************/

int	check_operator_error(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir)
{
	if (input[*i] == '|')
		return (handle_pipe_op(shell, input, i, new_redir));
	else if (input[*i] == '>')
		return (handle_greater_operator(shell, input, i, new_redir));
	else if (input[*i] == '<')
		return (handle_less_operator(shell, input, i, new_redir));
	return (0);
}