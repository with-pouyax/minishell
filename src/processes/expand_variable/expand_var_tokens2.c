/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_tokens2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:59:12 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/24 14:20:11 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  result ->  result is a pointer to a string that we store
//         the result in it
//     🏷  in_single_quote -> a pointer to an integer that we store the
//         single quote flag in it
//     🏷  in_double_quote -> a pointer to an integer that we store the
//         double quote flag in it
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- we allocate memory for the result and set it to an empty string.
//     2- if there is an error we print an error message and return 1.
//     3- we set the quote flags to 0.
//     4- we return 0.
//
/******************************************************************************/

int	initialize_expansion(t_shell_data *shell, char **result)
{
	shell->in_single_quote = 0;
	shell->in_double_quote = 0;
	*result = ft_strdup("");
	if (!*result)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
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
//     🏷  current_char -> the current character
//     🏷  i -> current index
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the current character is a single quote and we are not in a
//        double quote we toggle the single quote flag.
//     2- if the current character is a double quote and we are not in a
//        single quote we toggle the double quote flag.
//     3- we increment the index.
//
/******************************************************************************/

void	toggle_quotes_and_skip(t_shell_data *shell, char current_char, int *i)
{
	if (current_char == '\'' && !shell->in_double_quote)
		shell->in_single_quote = !shell->in_single_quote;
	else if (current_char == '\"' && !shell->in_single_quote)
		shell->in_double_quote = !shell->in_double_quote;
	(*i)++;
}



int	cleanup_and_return_null(char *result)
{
	free(result);
	return (1);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  result ->  result is a pointer to a string that we store
//         the result in it
//     🏷  str -> the string that we want to append to the result
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using ft_strjoin_free_both() we append the str to the result.
//        a- if there is an error we print an error message and return 1.
//     2- we put the temp string in the result.
//     3- we return 0.
//
/******************************************************************************/

int	append_str_to_result(char **result, char *str)
{
	char	*temp;

	(void)str;
	temp = ft_strjoin_free_both(*result, str);
	if (!temp)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (1);
	}
	*result = temp;
	return (0);
}
