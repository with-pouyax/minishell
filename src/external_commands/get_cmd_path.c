/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:45:05 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:20:57 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

// If the token starts with a '.', check if it's a valid path
// Check if it is "." or ".." and return NULL (not a valid command)
// Check if there is a valid path after the '.' (e.g., "./file")
char	*get_command_path(t_shell_data *shell, t_token *token)
{
	struct stat	path_stat;

	if (token->value[0] == '.')
	{
		if ((token->value[1] == '\0')
			|| (token->value[1] == '.' && token->value[2] == '\0'))
			return (NULL);
		if (token->value[1] == '/')
		{
			if (stat(token->value, &path_stat) == 0
				&& access(token->value, X_OK) == 0
				&& !S_ISDIR(path_stat.st_mode))
				return (ft_strdup(token->value));
		}
		return (NULL);
	}
	if (token->value[0] == '/')
	{
		if (stat(token->value, &path_stat) == 0
			&& access(token->value, X_OK) == 0 && !S_ISDIR(path_stat.st_mode))
			return (ft_strdup(token->value));
		return (NULL);
	}
	return (find_path_in_env(shell, token->value));
}
