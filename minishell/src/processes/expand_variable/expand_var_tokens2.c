/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_tokens2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:02:01 by pghajard          #+#    #+#             */
/*   Updated: 2025/01/22 14:04:41 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*****************************************************************************/
//                No explanation needed for this function
/*****************************************************************************/

int	handle_alpha_or_underscore(t_shell_data *shell, char *input, int *i, \
char **result)
{
	if (handle_variable_expansion(shell, input, i, result))
		return (1);
	return (0);
}

/*****************************************************************************/
// 💡 Notes:
//     we skip the digit
//
/******************************************************************************/
void	handle_digit(char *input, int *i)
{
	(void) input;
	(*i)++;
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  result -> our result
//
// 🔄 Returns   :  success status
/*****************************************************************************/
// 💡 Notes:
//     1- we append the dollar sign to the result.
//        a- if there is an error we print an error message and return 1.
//     2- using append_str_to_result() we append the dollar sign to the result.
//        a- if there is an error we free the temp string and return 1.
//     3- we return 0.
//
/******************************************************************************/
int	handle_default_case(char **result)
{
	char	*temp;

	temp = ft_strdup("$");
	if (!temp)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (1);
	}
	if (append_str_to_result(result, temp))
	{
		free(temp);
		return (1);
	}
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
//     🏷  result ->  result is a pointer to a string that we store
//         the result in it
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the current character is a ? we call handle_question_mark() to
//        handle the question mark.
//     2- else if the current character is an alphabet character or an
//        underscore we call handle_alpha_or_underscore() to handle the
//        alphabet character or the underscore.
//     3- else if the current character is a digit we call handle_digit() to
//        handle the digit.
//     4- if the current character is none of the above we call
//        handle_default_case() to handle the default case.
//
/******************************************************************************/

int	handle_dollar(t_shell_data *shell, char *input, int *i, char **result)
{
	(*i)++;
	if (input[*i] == '?')
	{
		if (handle_question_mark(shell, result, i))
			return (1);
	}
	else if (ft_isalpha((unsigned char)input[*i]) || input[*i] == '_')
	{
		if (handle_alpha_or_underscore(shell, input, i, result))
			return (1);
	}
	else if (ft_isdigit((unsigned char)input[*i]))
	{
		handle_digit(input, i);
	}
	else
	{
		if (handle_default_case(result))
			return (1);
	}
	return (0);
}

/*****************************************************************************/
//              No explanation needed for this function
/******************************************************************************/

void	handle_quote(t_shell_data *shell, char current_char, int *i)
{
	toggle_quotes_and_skip(shell, current_char, i);
}
