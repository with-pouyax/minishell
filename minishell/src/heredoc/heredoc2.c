/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:38:58 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:22:51 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	skip_until_operator_or_space(char *input, int *i)
{
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator_char(input[*i]))
		(*i)++;
}

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