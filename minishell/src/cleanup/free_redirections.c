/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:42:22 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 00:42:25 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*****************************************************************************/
//                            🚀 free_redirections 🚀
/*****************************************************************************/
// 🎯 Purpose  :    free redirections linked list
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  redirs -> linked list of redirections
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- using a while loop we are looping through our redirections linked
//        list nodes one by one.
//        a- we store the next node in next so we can free the current node
//           and don't lose access to the next node.
//        b- if the filename is not NULL we free it.
//        c- if the delimiter is not NULL we free it.
//        d- if the heredoc_file is not NULL we unlink it and free it.
//        e- we free the current node.
/******************************************************************************/

void	free_redirections(t_redirection *redirs)
{
	t_redirection	*current;
	t_redirection	*next;

	current = redirs;
	while (current)
	{
		next = current->next;
		if (current->filename)
			free(current->filename);
		if (current->delimiter)
			free(current->delimiter);
		if (current->heredoc_file)
		{
			unlink(current->heredoc_file);
			free(current->heredoc_file);
		}
		free(current);
		current = next;
	}
}
