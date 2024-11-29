#include "../minishell.h"

char	*check_and_return_path(const char *cmd, char **all_paths)
{
	char	*temp;
	char	*path_to_search;
	char	*final_path;

	final_path = NULL;
	while (*all_paths)
	{
		temp = ft_strjoin(*all_paths++, "/");
		if (!temp)
			exit(EXIT_FAILURE);
		path_to_search = ft_strjoin(temp, cmd);
		free(temp);
		if (!path_to_search)
			exit(EXIT_FAILURE);
		if (access(path_to_search, F_OK) == 0)
		{
			final_path = ft_strdup(path_to_search);
			free(path_to_search);
			break ;
		}
		free(path_to_search);
	}
	return (final_path);
}

char	**get_paths_from_env(char **env)
{
	int	i;
	char	**all_paths;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (env[i])
		all_paths = ft_split(env[i] + 5, ':');
	else
		return (NULL);
	return (all_paths);
}

char	*find_path_in_env(t_shell_data *shell, char *cmd)
{
	char	**all_paths;
	char	*path;
	int	i;

	i = 0;
	all_paths = get_paths_from_env(shell->envp);
	path = NULL;
	while (all_paths && all_paths[i])
	{
		path = check_and_return_path(cmd, all_paths);
		if (!path)
			break ;
		i++;
	}
	return (path);
}

char	*get_command_path(t_shell_data *shell, t_token *token)
{
	if (token->value[0] == '/' || token->value[0] == '.')
		return (ft_strdup(token->value));
	return (find_path_in_env(shell, token->value));
}
