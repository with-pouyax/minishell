/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_tokens3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:02:01 by pghajard          #+#    #+#             */
/*   Updated: 2025/01/22 14:05:11 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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
