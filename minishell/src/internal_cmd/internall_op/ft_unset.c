/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 09:43:55 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 09:44:16 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../internal_commands.h"

int	ft_unset(t_shell_data *shell, t_command *cmd)
{
	t_token *token;

	token = cmd->token_list->next;
	while (token)
	{
		if (is_valid_identifier(token->value, 0))
			remove_from_env(shell, token->value);
		else
			return (0);
		token = token->next;
	}
	return (0);
}
