/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:58:33 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/21 15:36:06 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :  making a file name
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  void
// 🔄 Returns   :  file_name
//
/*****************************************************************************/
// 💡 Notes:
//     1- using ft_itoa() we convert the counter to a string and store it in
//        counter_str.
//        a- if there is an error we print an error message and return NULL.
//     2- using ft_strjoin() we join the counter_str with the file name and
//        store it in filename.
//        a- if there is an error we print an error message
//     3- we free the counter_str.
//     4- we return the filename.
//
/******************************************************************************/

char	*generate_temp_filename(void)
{
	static int	counter = 0;
	char		*filename;
	char		*counter_str;

	counter_str = ft_itoa(counter++);
	if (!counter_str)
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
	if (!counter_str)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", counter_str);
	if (!filename)
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
	free(counter_str);
	return (filename);
}



