/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:31:36 by pghajard          #+#    #+#             */
/*   Updated: 2025/01/22 13:37:02 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  redirections ->
//     🏷  new_redir ->
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- if there is no redirection we put the new redirection into the
//        redirections.
//     2- if there is a redirection we loop over the redirections and put the
//        new redirection at the end.
//     3- we set the redirection number to the current redirection number + 1.
//
/******************************************************************************/

void	add_redirection(t_redirection **redirections, t_redirection *new_redir)
{
	t_redirection	*current;

	if (!*redirections)
		*redirections = new_redir;
	else
	{
		current = *redirections;
		while (current->next)
			current = current->next;
		current->next = new_redir;
		new_redir->redir_number = current->redir_number + 1;
	}
}
/*****************************************************************************/
//                  No explanation needed for this function
/*****************************************************************************/

int	starts_with_operator_char(char c)
{
	return (c == '<' || c == '>' || c == '|');
}
