/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:54:40 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:22:32 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	syntax_error_newline(void)
{
	ft_putstr_fd("syntax error near unexpected token "
		"`newline'\n", STDERR_FILENO);
	return (1);
}

/*****************************************************************************/
//                    No explanation needed for this function
/******************************************************************************/

int	heredoc_open_error(char *tmp_filename)
{
	write(STDERR_FILENO, "minishell: failed to open temporary file\n", 41);
	free(tmp_filename);
	return (1);
}
