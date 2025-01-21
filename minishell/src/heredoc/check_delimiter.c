/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_delimiter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:52:06 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/21 15:35:33 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :   here we check if the delimiter is quoted or not.
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  redir         ->
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- we put the delimiter from the redirection structure into delimiter.
//     2- if the delimiter first character is a single quote or a double quote
//        a- we extract the unquoted delimiter.
//        b- if there is an error extracting the unquoted delimiter we
//        return -1.
//        c- we free the delimiter inside the redirection structure.
//        d- we put the unquoted delimiter into the redirection structure.
//        e- we return 1, which means the delimiter unquoted successfully.
//     3- if the delimiter is not unquoted we return 0.
//
//
/******************************************************************************/

int	check_delimiter_quotes(t_redirection *redir)
{
	char	*delimiter;
	char	*unquoted_delimiter;

	delimiter = redir->delimiter;
	if (delimiter[0] == '\'' || delimiter[0] == '\"')
	{
		unquoted_delimiter = ft_substr(delimiter, 1, ft_strlen(delimiter) - 2);
		if (!unquoted_delimiter)
			return (-1);
		free(redir->delimiter);
		redir->delimiter = unquoted_delimiter;
		return (1);
	}
	return (0);
}
