/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:55:00 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/22 12:42:48 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  line -> the line we want to handle
//     🏷  fd -> the file descriptor
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using expand_and_write_line() we expand and write the line and store
//        the expanded line in temp.
//        a- if there is an error we return 1.
//     2- we write the temp to the file descriptor.
//     3- we write a newline character to the file descriptor.
//     4- we free the temp and return 0.
//
/******************************************************************************/

int	expand_and_write_line(t_shell_data *shell ,char *line, int fd)
{
	char	*temp;

	temp = expand_variables_in_heredoc(shell, line);
	if (!temp)
		return (1);
	write(fd, temp, ft_strlen(temp));
	write(fd, "\n", 1);
	free(temp);
	return (0);
}
