/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:55:00 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/22 21:38:42 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  input -> user input
//     🏷  i -> current index
//     🏷  result -> we store the result here
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the current character is a single quote and we are not in a
//        double quote or the current character is a double quote and we are
//        not in a single quote.
//        a- using toggle_quotes_and_append() we toggle the quotes and append
//           the current character to the result.
//           I- if there is an error we return -1.
//     2- else if the current character is a dollar sign and we are not in a
//        single quote.
//        a- using handle_dollar() we handle the dollar sign.
//           I- if there is an error we return -1.
//     3- if the current character is a literal character (not a quote or a
//        dollar sign) we append the literal character to the result using
//        append_literal_char().
//           I- if there is an error we return -1.
//     4- we return 0.
//
/******************************************************************************/

int	process_heredoc_expansion(t_shell_data *shell, char *input, \
int *i, char **result)
{
	while (input[*i])
	{
		if ((input[*i] == '\'' && !shell->in_double_quote) || \
			(input[*i] == '\"' && !shell->in_single_quote))
		{
			if (toggle_quotes_and_append(shell, input[*i], i, result))
				return (-1);
		}
		else if (input[*i] == '$' && !shell->in_single_quote)
		{
			if (handle_dollar(shell, input, i, result))
				return (-1);
		}
		else
		{
			if (append_literal_char(input, i, result))
				return (-1);
		}
	}
	return (0);
}

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
//     1-
//
/******************************************************************************/

char	*expand_variables_in_heredoc(t_shell_data *shell, char *input)
{
	char	*result;
	int		i;

	if (initialize_expansion(shell, &result))
		return (NULL);
	i = 0;
	if (process_heredoc_expansion(shell, input, &i, &result))
	{
		free(result);
		return (NULL);
	}
	return (result);
}
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
