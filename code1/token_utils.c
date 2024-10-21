/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by yourname          #+#    #+#             */
/*   Updated: 2024/10/21 15:50:25 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(char *token_value, t_token **token_list, int *index, int is_operator)
{
	t_token	*new_token;
	t_token	*current;

	new_token = malloc(sizeof(t_token));
	new_token->value = token_value;
	new_token->index = (*index)++;
	new_token->is_operator = is_operator;
	new_token->is_command = 0;
	new_token->is_flag = 0;
	new_token->is_int = 0;
	new_token->var_not_found = 0;
	new_token->next = NULL;
	if (!*token_list)
		*token_list = new_token;
	else
	{
		current = *token_list;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
}

int	is_operator(char *token)
{
	if (!ft_strcmp(token, "|") || !ft_strcmp(token, "<") || !ft_strcmp(token, ">") ||
		!ft_strcmp(token, ">>") || !ft_strcmp(token, "<<"))
		return (1);
	return (0);
}

int	is_internal_command(char *token)
{
	if (!ft_strcmp(token, "echo") || !ft_strcmp(token, "cd") ||
		!ft_strcmp(token, "pwd") || !ft_strcmp(token, "export") ||
		!ft_strcmp(token, "unset") || !ft_strcmp(token, "env") ||
		!ft_strcmp(token, "exit"))
		return (1);
	return (0);
}
