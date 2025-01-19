/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:52:45 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 00:54:25 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
