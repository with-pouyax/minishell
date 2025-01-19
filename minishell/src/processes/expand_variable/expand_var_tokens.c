/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:59:12 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 13:09:57 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  result ->  result is a pointer to a string that we store
//         the result in it
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using ft_itoa() we convert the exit status from an integer to a
//        string and store it in exit_str.
//        a- if there is an error we print an error message and return 1.
//     2- using append_str_to_result() we append the exit status to the result.
//        a- if there is an error we return 1.
//     3- we return 0.
//
/******************************************************************************/

int	handle_exit_status(t_shell_data *shell, char **result)
{
	char	*exit_str;

	(void)shell;
	exit_str = ft_itoa(shell->exit_status);
	if (!exit_str)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (1);
	}
	if (append_str_to_result(result, exit_str))
		return (1);
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//     🏷  input -> user input
//     🏷  i -> current index
//     🏷  result ->
//
/*****************************************************************************/
// 💡 Notes:
//     1- we set the var_not_found_flag to 0 because we didn't find the
//        variable yet.
//     2- using get_variable_value() we get the value of the variable and
//        store it in var_value.
//        a- if there is an error we return 1.
//     3- using append_str_to_result() we append the value of the variable to
//        the result.
//        a- if there is an error we return 1.
//     4- we return 0.
//
/******************************************************************************/

int	handle_variable_expansion(t_shell_data *shell, char *input, \
int *i, char **result)
{
	char	*var_value;
	int		var_not_found_flag;

	var_not_found_flag = 0;
	var_value = get_variable_value(shell, input, i, &var_not_found_flag);
	if (!var_value)
		return (1);
	if (append_str_to_result(result, var_value))
		return (1);
	return (0);
}

/*****************************************************************************/
//                 No explanation needed for this function
/******************************************************************************/

int	handle_question_mark(t_shell_data *shell, char **result, int *i)
{
	(*i)++;
	if (handle_exit_status(shell, result))
		return (1);
	return (0);
}

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
