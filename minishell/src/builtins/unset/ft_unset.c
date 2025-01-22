/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 09:43:55 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/21 12:53:28 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/builtins.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//     🏷  cmd -> our command structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we get the next token from the token list.
//     2- we loop through the tokens using a while loop.
//         a- using is_valid_identifier() we check if the token is a valid
//            identifier.
//            I- if it using remove_from_env() we remove the token from the
//               environment.
//            II- if it is not we return 0.
//
//
/******************************************************************************/

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
