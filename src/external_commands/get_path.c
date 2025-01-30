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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_and_return_path.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourname <yourname@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:00:00 by yourname          #+#    #+#             */
/*   Updated: 2025/01/30 10:00:00 by yourname         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


static int	is_directory1(char *path_to_search)
{
	struct stat	st;

	if (stat(path_to_search, &st) == 0 && S_ISDIR(st.st_mode))
		return (1);
	return (0);
}

static char	*handle_valid_path1(char *path_to_search)
{
	char	*final_path;

	final_path = ft_strdup(path_to_search);
	free(path_to_search);
	return (final_path);
}

char	*check_and_return_path(const char *cmd, char **all_paths)
{
	int		i;
	char	*path_to_search, *final_path;

	i = 0; final_path = NULL;
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


char	**get_paths_from_env(t_shell_data *shell, char **env)
{
	int		i;
	char	**all_paths;

	i = 0;
	all_paths = NULL;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (env[i])
		all_paths = ft_split(env[i] + 5, ':');
	else if (shell->prev_dir)
		all_paths = ft_split(shell->prev_dir, ':');
	return (all_paths);
}

char	*find_path_in_env(t_shell_data *shell, char *cmd)
{
	char	**all_paths;
	char	*path;

	path = NULL;
	all_paths = get_paths_from_env(shell, shell->envp);
	if (!all_paths)
		return (NULL);
	path = check_and_return_path(cmd, all_paths);
	if (!path)
	{
		free_paths(all_paths);
		return (NULL);
	}
	free_paths(all_paths);
	return (path);
}

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

