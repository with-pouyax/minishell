#include "../minishell.h"



char	**get_paths_from_env(t_shell_data *shell, char **env)
{
	int	i;
	char	**all_paths;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (env[i])
		all_paths = ft_split(env[i] + 5, ':');
	else
		all_paths = NULL;
    if (!all_paths)
    {
        free_paths(all_paths); // Ensure memory is freed if all_paths was allocated
        all_paths = ft_split(shell->prev_dir, ':'); // Split prev_dir into paths
    }
    return (all_paths);
}

char	*find_path_in_env(t_shell_data *shell, char *cmd)
{
	char	**all_paths;
	char	*path;
	int	i;

	i = 0;
	all_paths = get_paths_from_env(shell, shell->envp);
	path = NULL;
	if (!all_paths)
    {
        // write_error(cmd, "PATH not set");
        return (NULL);
    }
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
