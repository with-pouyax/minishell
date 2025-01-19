/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:52:45 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 12:40:29 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  input -> user input
//     🏷  i -> current index
//
// 🔄 Returns   :  var_name
/*****************************************************************************/
// 💡 Notes:
//     1- using get_var_name_len() we get the length of the variable name and
//        store it in var_len.
//     2- using ft_substr() we extract the variable name from the input and
//        store it in var_name.
//        a- if there is an error we print an error message and return NULL.
//     3- we increment the index by var_len.
//
/******************************************************************************/

char	*extract_var_name(char *input, int *i)
{
	char	*var_name;
	int		var_len;

	var_len = get_var_name_len(&input[*i]);
	var_name = ft_substr(input, *i, var_len);
	if (!var_name)
	{
		ft_putstr_fd("minishell: memory allocation fail\n", STDERR_FILENO);
		return (NULL);
	}
	*i += var_len;
	return (var_name);
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//     🏷  var_name -> the name of the variable
//     🏷  var_not_found_flag -> a pointer to an integer that we store the
//        variable not found flag in it
//
// 🔄 Returns   :  var_value
/*****************************************************************************/
// 💡 Notes:
//     1- using getenv_from_envp() we get the value of the variable from the
//        environment variables and store it in var_value.
//        a- if we find the var value:
//           I- we duplicate the var_value and store it in var_value.
//              A- if there is an error duplicating the var_value we print
//                 an error message.
//     2- if we don't find the var value:
//        a- we duplicate an empty string and store it in var_value.
//           I- if there is an error duplicating the var_value we print
//              an error message.
//        b- we set the variable not found flag to 1.
//    3- we return var_value.
//
/******************************************************************************/

char	*retrieve_var_value(t_shell_data *shell, char *var_name, \
int *var_not_found_flag)
{
	char	*var_value;

	var_value = getenv_from_envp(shell, var_name);
	if (var_value)
	{
		var_value = ft_strdup(var_value);
		if (!var_value)
			ft_putstr_fd("minishell: memory allocation fail\n", STDERR_FILENO);
	}
	else
	{
		var_value = ft_strdup("");
		if (!var_value)
			ft_putstr_fd("minishell: memory allocation fail\n", STDERR_FILENO);
		*var_not_found_flag = 1;
	}
	return (var_value);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//     🏷  input -> user input
//     🏷  i -> current index
//     🏷  var_not_found_flag -> a pointer to an integer that we store the
//
// 🔄 Returns   :  the value of the variable
/*****************************************************************************/
// 💡 Notes:
//     1- using extract_var_name() we extract the variable name from the input
//        and store it in var_name, by var_name we mean the name after the
//        dollar sign.
//        a- if there is an error we return NULL.
//     2- using retrieve_var_value() we get the value of the var_name and store
//        it in var_value.
//     3- we free var_name.
//     4- we return var_value.
//
/******************************************************************************/

char	*get_variable_value(t_shell_data *shell, char *input, int *i, \
int *var_not_found_flag)
{
	char	*var_name;
	char	*var_value;

	var_name = extract_var_name(input, i);
	if (!var_name)
		return (NULL);
	var_value = retrieve_var_value(shell, var_name, var_not_found_flag);
	free(var_name);
	return (var_value);
}
