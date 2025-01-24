/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 11:05:57 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:35:25 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :  parse and tokenize the input
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we call split_cmd_tokenize() to split the input into commands
//        and tokenize them.
//        a- if there is an error we return 1.
//     2- if there are no commands we return 1.
//     3- if there are error flags we free the commands and NULL them and
//        return 1.
//     4- we call parse_tokens() to parse the tokens.
//     5- what is the difference between split_cmd_tokenize()
//        and parse_tokens()?
//        [?????????????????????]
//     6- using validate_operators() we validate the operators in the commands
//        a- if there is an error we return 1.
//     7- if everything is fine we return 0.
/******************************************************************************/

int	preprocess_input(t_shell_data *shell)
{
	shell->commands = NULL;
	if (split_cmd_tokenize(shell) != 0)
		return (1);
	if (!shell->commands)
		return (1);
	if (shell->error_flag)
	{
		free_commands(shell);
		shell->commands = NULL;
		return (1);
	}
	parse_tokens(shell);
	if (validate_operators(shell))
		return (1);
	return (0);
}
