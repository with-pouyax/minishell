/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:59:12 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:31:23 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"
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
//     1- we set shell->expanded to 1 because later we need to know if we
//        expanded the input or not.
//
/******************************************************************************/

int	handle_dollar_expansion(t_shell_data *shell, char *input, \
									int *i, char **result)
{
	shell->double_quoted = shell->in_double_quote;
	shell->expanded = 1;
	if (handle_dollar(shell, input, i, result))
	{
		free(*result);
		return (1);
	}
	return (0);
}

/*****************************************************************************/
//             No explanation needed for this function
/******************************************************************************/

int	handle_literal_character(char *input, int *i, char **result)
{
	if (append_literal_char(input, i, result))
	{
		free(*result);
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
//     1- if the current character is a single quote or a double quote and
//        we are not in the opposite quote we call handle_quote() to toggle
//        (remove) the quote flag.
//     2- if the current character is a dollar sign and we are not in a
//        single quote we call handle_dollar_expansion() to handle the
//        dollar sign expansion.
//     3- if the current character is a literal character (not a quote or a
//        dollar sign) we call handle_literal_character() to handle the
//        literal character.
//
/******************************************************************************/

int	process_char(t_shell_data *shell, char *input, int *i, char **result)
{
	char	current_char;

	current_char = input[*i];
	if ((current_char == '\'' && !shell->in_double_quote) || \
		(current_char == '\"' && !shell->in_single_quote))
	{
		handle_quote(shell, current_char, i);
	}
	else if (current_char == '$' && !shell->in_single_quote)
	{
		if (handle_dollar_expansion(shell, input, i, result))
			return (1);
	}
	else
	{
		if (handle_literal_character(input, i, result))
			return (1);
	}
	return (0);
}

/*****************************************************************************/
//         No explanation needed for this function
/*****************************************************************************/

int	initialize_and_check(t_shell_data *shell, char **result)
{
	if (initialize_expansion(shell, result))
		return (1);
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  input -> user input
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using initialize_and_check() we allocate memory for the result and
//        set the quote flags to 0.
//        a- if there is an error we return NULL.
//     2- using a while loop we iterate over the input characters
//        a- using process_char() we process the current character, by
//           processing we mean we check if the character is a quote or a
//           dollar sign or a literal character and we handle it accordingly
//           and store the result in the result string.
//           I- if there is an error we return NULL.
//     3- at the end we return the result.
//
/******************************************************************************/

char	*expand_variables_in_token(t_shell_data *shell, char *input)
{
	char	*result;
	int		i;

	if (initialize_and_check(shell, &result))
		return (NULL);
	i = 0;
	while (input[i])
	{
		if (process_char(shell, input, &i, &result))
			return (NULL);
	}
	return (result);
}
