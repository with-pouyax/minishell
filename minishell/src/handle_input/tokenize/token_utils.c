/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:05:39 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/22 13:37:10 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  token_value ->
//     🏷  token_list ->
//     🏷  index ->
//     🏷  is_operator ->
//
// 🔄 Returns   :  1 if op is redirection, else 0
//
/*****************************************************************************/
// 💡 Notes:
//     1- we allocate memory for a new token.
//        a- if there is an error we print an error message and return 1.
//     2- using initialize_new_token() we initialize the new token.
//     3- if there is no token_list we put the new token in the token_list.
//     4- if there is a token_list we loop over the token_list and put the new
//        token at the end.
//     5- at the end we return 0.
//
/******************************************************************************/

int	add_token(char *token_value, t_token **token_list,
		int *index, int is_operator)
{
	t_token	*new_token;
	t_token	*current;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (1);
	}
	initialize_new_token(new_token, token_value, index, is_operator);
	if (!*token_list)
		*token_list = new_token;
	else
	{
		current = *token_list;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
	return (0);
}

/*****************************************************************************/
//                No explanation needed for this function
/******************************************************************************/

void	initialize_new_token(t_token *new_token, char *token_value, \
int *index, int is_operator)
{
	new_token->value = token_value;
	new_token->original_value = NULL;
	new_token->index = (*index)++;
	new_token->is_operator = is_operator;
	new_token->is_command = 0;
	new_token->is_flag = 0;
	new_token->is_int = 0;
	new_token->var_not_found = 0;
	new_token->wrong_operator = 0;
	new_token->is_heredoc = 0;
	new_token->heredoc_delimiter = NULL;
	new_token->heredoc_file = NULL;
	new_token->is_end = 0;
	new_token->next = NULL;
}
