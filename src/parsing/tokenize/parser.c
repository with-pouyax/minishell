/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 09:57:16 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:26:12 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   : void
//
/*****************************************************************************/
// 💡 Notes:
//     1- we iterate over the commands using a while loop.
//        a- if the token index is 0 and the token is not an operator
//           I- we set the is_command flag to 1.
//           II- using is_internal_command() we check if the token is an
//           internal command.
//               A- if it is an internal command we set the is_int flag to 1.
//        b- if the token starts with a '-'
//           I- we set the is_flag flag to 1.
//        c- we move to the next token.
//    2- now that we are out of the loop we have parsed the tokens so we
//       go to the next command.
/******************************************************************************/

void	parse_tokens(t_shell_data *shell)
{
	t_command	*cmd;
	t_token		*token;

	cmd = shell->commands;
	while (cmd)
	{
		token = cmd->token_list;
		while (token)
		{
			if (token->index == 0 && !token->is_operator)
			{
				token->is_command = 1;
				if (is_internal_command(token->value))
					token->is_int = 1;
			}
			else if (ft_strncmp(token->value, "-", 1) == 0)
				token->is_flag = 1;
			token = token->next;
		}
		cmd = cmd->next;
	}
}
