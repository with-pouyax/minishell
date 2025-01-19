/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_delimiter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:52:06 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 00:52:24 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


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
