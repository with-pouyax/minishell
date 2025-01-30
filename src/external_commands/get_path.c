/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:59:12 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/30 15:42:51 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Join two strings and free the first one after joining
char	*join_path(const char *prefix, const char *suffix)
{
	char	*result;
	char	*temp;

	(void)prefix;
	temp = ft_strjoin(prefix, "/");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, suffix);
	free(temp);
	return (result);
}

// Check if a path exists and is not a directory
int	is_valid_file(const char *path)
{
	struct stat	st;

	return (stat(path, &st) == 0 && !S_ISDIR(st.st_mode));
}

int	is_directory1(char *path_to_search)
{
	struct stat	st;

	if (stat(path_to_search, &st) == 0 && S_ISDIR(st.st_mode))
		return (1);
	return (0);
}

char	*handle_valid_path1(char *path_to_search)
{
	char	*final_path;

	final_path = ft_strdup(path_to_search);
	free(path_to_search);
	return (final_path);
}

char	*check_and_return_path(const char *cmd, char **all_paths)
{
	int		i;
	char	*path_to_search;
	char	*final_path;

	i = 0;
	final_path = NULL;
	while (all_paths[i])
	{
		path_to_search = join_path(all_paths[i], cmd);
		if (!path_to_search)
			return (NULL);
		if (access(path_to_search, F_OK) == 0 && is_valid_file(path_to_search))
		{
			final_path = handle_valid_path1(path_to_search);
			break ;
		}
		if (is_directory1(path_to_search))
		{
			free(path_to_search);
			break ;
		}
		free(path_to_search);
		i++;
	}
	return (final_path);
}
