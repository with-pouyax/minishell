/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 11:04:09 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/21 15:40:31 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//     🏷  name -> the name of the variable
//
// 🔄 Returns   : var value
/*****************************************************************************/
// 💡 Notes:
//     1- we find the length of the variable name and store it in len.
//     2- using a while loop we iterate over the envp array.
//        a- each time we check if the variable name is equal to the name and
//           the character after the name is an equal sign.
//           I- if the condition is true we return the value of the variable.
//     3- if we finish the loop without finding the variable we return NULL.
//
/******************************************************************************/
char	*getenv_from_envp(t_shell_data *shell, char *name)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], name, len) == 0
			&& shell->envp[i][len] == '=')
			return (shell->envp[i] + len + 1);
		i++;
	}
	return (NULL);
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
// 💡 Notes:
//     1- a variable name can finish with underscore.
//
/******************************************************************************/
int	get_var_name_len(char *str)
{
	int	len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}



/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷
//
// 🔄 Returns   :
/*****************************************************************************/
// 💡 Notes:
//     - str is a pointer to a string that we store the character in it.
//     1- using ft_substr() we get the character at the current index and store
//        it in str.
//     2- if substr() fails we print an error message
//     3- we move the index to the next character.
//     4- return the str.
//
/******************************************************************************/
char	*get_literal_char(char *input, int *i)
{
	char	*str;

	(void)input;
	str = ft_substr(input, *i, 1);
	if (!str)
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
	(*i)++;
	return (str);
}



/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷
//
// 🔄 Returns   : success status
/*****************************************************************************/
// 💡 Notes:
//     -literal characters are characters that are characters that are not
//      special characters like quotes or dollar signs and they are not
//      variables.
//      1- using get_literal_char() we get the literal character and store it
//         in temp.
//         a- if there is an error we return 1.
//      2- using ft_strjoin_free_both() we append the temp to the result.
//         a- if there is an error we print an error message and return 1.
//      3- we return 0.
//
/******************************************************************************/

int	append_literal_char(char *input, int *i, char **result)
{
	char	*temp;

	temp = get_literal_char(input, i);
	if (!temp)
		return (1);
	*result = ft_strjoin_free_both(*result, temp);
	if (!*result)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}
