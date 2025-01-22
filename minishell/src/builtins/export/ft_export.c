/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 09:42:44 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/21 12:53:14 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../includes/builtins.h"


/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we go to the next token in the command because the first token is
//        the command itself.
//        a- if there is no token we print the sorted environment and return 0.
//     2- we loop through the tokens using a while loop.
//        a- using is_valid_identifier() we check if the token is a valid
//           identifier.(meaning it does not contain any special characters)
//           i- if it is a valid identifier we add it to the environment using
//              add_to_env().
//        b- if is not a valid identifier we print an error message and
//        return 1.
//     3- we return 0.
/******************************************************************************/

int	ft_export(t_shell_data *shell, t_command *cmd)
{
	t_token	*token;

	token = cmd->token_list->next;
	if (!token)
	{
		print_sorted_env(shell);
		shell->exit_status = 0;
		return (0);
	}
	while (token)
	{
		if (is_valid_identifier(token->value, 1))
			add_to_env(shell, token->value);
		else
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(token->value, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			shell->exit_status = 1;
			return (1);
		}
		token = token->next;
	}
	return (0);
}
