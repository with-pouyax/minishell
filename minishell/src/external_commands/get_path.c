#include "../minishell.h"

// Join two strings and free the first one after joining
char *join_path(const char *prefix, const char *suffix)
{
    char *result;
    char *temp;
    
    temp = ft_strjoin(prefix, "/");
    if (!temp)
        return (NULL);
    result = ft_strjoin(temp, suffix);
    free(temp);
    return (result);
}

// Check if a path exists and is not a directory
int is_valid_file(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0 && !S_ISDIR(st.st_mode));
}

char *check_and_return_path(const char *cmd, char **all_paths)
{
    char *path_to_search;
    char *final_path;
    struct stat st;

    final_path = NULL;
    while (*all_paths)
    {
        path_to_search = join_path(*all_paths++, cmd);
        if (!path_to_search)
            return (NULL);
        if (access(path_to_search, F_OK) == 0 && is_valid_file(path_to_search))
        {
            final_path = ft_strdup(path_to_search);
            free(path_to_search);
            break;
        }
        if (stat(path_to_search, &st) == 0 && S_ISDIR(st.st_mode)) // Check if it's a directory
        {
            free(path_to_search);
            break;
        }
        free(path_to_search);
    }
    return (final_path);
}


char	**get_paths_from_env(t_shell_data *shell, char **env)
{
	int	i;
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
	int	i;

	i = 0;
    path = NULL;
	all_paths = get_paths_from_env(shell, shell->envp);
	if (!all_paths)
        return (NULL);
	while (all_paths && all_paths[i])
	{
		path = check_and_return_path(cmd, all_paths);
		if (!path)
			break ;
		i++;
	}
	free_paths(all_paths);
	return (path);
}

char	*get_command_path(t_shell_data *shell, t_token *token)
{
	if (token->value[0] == '/' || token->value[0] == '.')
		return (ft_strdup(token->value));
	return (find_path_in_env(shell, token->value));
}
