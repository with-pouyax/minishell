/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 22:44:33 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/22 22:44:47 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

/*****************************************************************************/
// 🎯 Purpose  :  prepare the redirection and store it in new_redir linked list
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  cmd -> our command linked list
//     🏷  new_redir -> the redirection we want to prepare
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using create_new_redirection() we create a new redirection and store
//        it in new_redir linked list.
//     2- we free op because we have already stored it in new_redir.
//     3- we set the current operator to NULL because we have already stored it
//        in new_redir.
//     4- if we successfully created the new redirection we return 0, else 1.
//
/******************************************************************************/

int	prepare_redirection(t_command *cmd, t_redirection **new_redir)
{
	char	*op;

	op = cmd->current_op;
	if (!op)
		return (1);
	*new_redir = create_new_redirection(op);
	free(op);
	cmd->current_op = NULL;
	return (!(*new_redir));
}

/*****************************************************************************/
// 🎯 Purpose  :  handle redirection
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  input -> user input
//     🏷  i -> index of the current character in the input
//     🏷  cmd -> our command linked list
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- we set the filename_or_delimiter to NULL.
//     2- using prepare_redirection() we prepare the redirection.
//        a- if there is an error we return 1.
//     3- using skip_whitespace() we skip the whitespaces in the input.
//     4- using check_operator_error() we check if there is an operator error.
//        a- if there is an error we return 1.
//     5- using process_filename_or_delimiter() we process the filename
//        or delimiter.
//        a- if there is an error we return 1.
//     6- using finalize_redirection() we finalize the redirection [???]
//        a- if there is an error we return 1.
//     7- using add_redirection() we add this new_redir to the redirections
//        linked list.
//     8- if everything is fine we return 0.
//
/******************************************************************************/

int handle_redirection(t_shell_data *shell, char *input, int *i, \
t_command *cmd)
{
	t_redirection	*new_redir;

	shell->filename_or_delimiter = NULL;
	if (prepare_redirection(cmd, &new_redir))
		return (1);

	skip_whitespace(input, i);
	if (check_operator_error(shell, input, i, new_redir))
		return (1);
	if (process_filename_or_delimiter(shell, input, i, new_redir))
		return (1);
	if (finalize_redirection(shell, new_redir))
		return (1);
	add_redirection(&(cmd->redirections), new_redir);
	return (0);
}
/*****************************************************************************/
// 🎯 Purpose  :  check if op is a redirection
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  op -> the character we recieved as operator.
//
// 🔄 Returns   :  1 if op is redirection, else 0
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the op is <, >, >>, or << we return 1 else 0.
//
/******************************************************************************/
int	is_redirection_operator(char *op)
{
	return (!ft_strcmp(op, "<") || !ft_strcmp(op, ">") || \
	!ft_strcmp(op, ">>") || !ft_strcmp(op, "<<"));
}