/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 22:43:53 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:27:19 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"


/*****************************************************************************/
// 🎯 Purpose  :  skip_white spaces
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  input
//     🏷  i
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- using a while loop we iterate over the input characters as long as
//        the current character is a white space we increment the index.
//
/******************************************************************************/

void	skip_whitespace(char *input, int *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}

/*****************************************************************************/
// 🎯 Purpose  :  create a new redirection and store it in new_redir linked list
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  op -> our operation
// 🔄 Returns   :  our new redirection
//
/*****************************************************************************/
// 💡 Notes:
//     1- we allocate memory for our new redirection.
//        a- if there is an error we print an error message and return NULL.
//     2- using ft_bzero() we set the new redirection elements to 0.
//     3- using ft_strcmp() we check the operation and set the type of the
//        propper redirection.
//     4- we return the new redirection.
//
/******************************************************************************/

t_redirection	*create_new_redirection(char *op)
{
	t_redirection	*new_redir;

	new_redir = malloc(sizeof(t_redirection));
	if (!new_redir)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (NULL);
	}
	ft_bzero(new_redir, sizeof(t_redirection));
	if (!ft_strcmp(op, "<"))
		new_redir->type = REDIR_INPUT;
	else if (!ft_strcmp(op, ">"))
		new_redir->type = REDIR_OUTPUT;
	else if (!ft_strcmp(op, ">>"))
		new_redir->type = REDIR_APPEND;
	else if (!ft_strcmp(op, "<<"))
		new_redir->type = REDIR_HEREDOC;

	return (new_redir);
}

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