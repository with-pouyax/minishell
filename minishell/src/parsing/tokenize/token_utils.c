/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:05:39 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/22 22:44:27 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"




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
