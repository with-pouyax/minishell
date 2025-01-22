/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:43:02 by pghajard          #+#    #+#             */
/*   Updated: 2025/01/22 12:43:21 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*****************************************************************************/
//              No explanation needed for this function
/*****************************************************************************/

void	skip_until_operator_or_space(char *input, int *i)
{
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator_char(input[*i]))
		(*i)++;
}

/*****************************************************************************/
//              No explanation needed for this function
/*****************************************************************************/

char	*get_line_from_input(char *input, int *index)
{
	int		start;
	char	*line;

	start = *index;
	while (input[*index] && input[*index] != '\n')
		(*index)++;
	line = ft_substr(input, start, *index - start);
	if (input[*index] == '\n')
		(*index)++;
	return (line);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structurer
//     🏷  current_char ->
//     🏷  i ->
//     🏷  result ->
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the current character is a single quote and we are not in a
//        double quote
//        a- we toggle the single quote flag.
//     2- else if the current character is a double quote and we are not in a
//        single quote
//        a- we toggle the double quote flag.
//     3- using add_char_to_token() we append the current character to the
//        result.
//        a- if there is an error we return 1.
//     4- we increment the index.
//     5- we return 0.
//
/******************************************************************************/

int	toggle_quotes_and_append(t_shell_data *shell, char current_char, \
int *i, char **result)
{
	if (current_char == '\'' && !shell->in_double_quote)
		shell->in_single_quote = !shell->in_single_quote;
	else if (current_char == '\"' && !shell->in_single_quote)
		shell->in_double_quote = !shell->in_double_quote;
	if (add_char_to_token(result, current_char))
		return (1);
	(*i)++;
	return (0);
}

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