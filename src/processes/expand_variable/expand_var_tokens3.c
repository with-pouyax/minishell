/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_tokens3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:59:12 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/24 14:11:00 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
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
	{
		free(var_value);
		return (1);
	}
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
