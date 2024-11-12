// internal_commands_utils.c
#include "internal_commands.h"

/* Check if a string is numeric */
int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/* Validate identifier for export and unset */
int	is_valid_identifier(const char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/* Add or update environment variable */
void	add_to_env(const char *str)
{
	int		i;
	char	*key;
	char	*new_var;
	int		len;

	// Extract key from `str` up to the '=' character
	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	key = ft_substr(str, 0, len);
	if (!key)
	{
		ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
		g_data.exit_status = 1;
		return;
	}

	// Check if the key already exists in the environment variables
	i = 0;
	while (g_data.envp[i])
	{
		if (ft_strncmp(g_data.envp[i], key, len) == 0 && g_data.envp[i][len] == '=')
		{
			// Update existing variable with new value
			free(g_data.envp[i]);
			g_data.envp[i] = ft_strdup(str);
			free(key);
			if (!g_data.envp[i])
			{
				ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
				g_data.exit_status = 1;
			}
			return;
		}
		i++;
	}
	free(key);

	// Add new variable if it doesn't exist
	new_var = ft_strdup(str);
	if (!new_var)
	{
		ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
		g_data.exit_status = 1;
		return;
	}

	// Resize envp to hold the new variable
	int	env_size = i;
	char **new_envp = malloc(sizeof(char *) * (env_size + 2));
	if (!new_envp)
	{
		ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
		free(new_var);
		g_data.exit_status = 1;
		return;
	}

	// Copy over existing variables
	for (i = 0; g_data.envp[i]; i++)
		new_envp[i] = g_data.envp[i];
	new_envp[i] = new_var;
	new_envp[i + 1] = NULL;

	free(g_data.envp);
	g_data.envp = new_envp;
	g_data.exit_status = 0;
}

/* Remove environment variable */
void	remove_from_env(const char *name)
{
	int	i, j;
	int	len;
	char	**new_envp;

	len = ft_strlen(name);
	i = 0;
	while (g_data.envp[i])
		i++;

	new_envp = malloc(sizeof(char *) * (i + 1));
	if (!new_envp)
	{
		ft_putstr_fd("minishell: unset: allocation error\n", STDERR_FILENO);
		g_data.exit_status = 1;
		return;
	}

	i = 0;
	j = 0;
	while (g_data.envp[i])
	{
		if (!(ft_strncmp(g_data.envp[i], name, len) == 0 &&
			  (g_data.envp[i][len] == '=' || g_data.envp[i][len] == '\0')))
		{
			new_envp[j++] = g_data.envp[i];
		}
		else
		{
			free(g_data.envp[i]);
		}
		i++;
	}
	new_envp[j] = NULL;
	free(g_data.envp);
	g_data.envp = new_envp;
}

/* Print sorted environment variables */
void	print_sorted_env(void)
{
	int		i, j;
	int		env_size;
	char	**sorted_envp;
	char	*temp;

	// Count environment variables
	env_size = 0;
	while (g_data.envp[env_size])
		env_size++;

	// Duplicate envp
	sorted_envp = malloc(sizeof(char *) * (env_size + 1));
	if (!sorted_envp)
	{
		ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
		g_data.exit_status = 1;
		return;
	}

	for (i = 0; i < env_size; i++)
		sorted_envp[i] = ft_strdup(g_data.envp[i]);
	sorted_envp[env_size] = NULL;

	// Simple bubble sort
	for (i = 0; i < env_size - 1; i++)
	{
		for (j = i + 1; j < env_size; j++)
		{
			if (ft_strcmp(sorted_envp[i], sorted_envp[j]) > 0)
			{
				temp = sorted_envp[i];
				sorted_envp[i] = sorted_envp[j];
				sorted_envp[j] = temp;
			}
		}
	}

	// Print sorted envp
	for (i = 0; sorted_envp[i]; i++)
	{
		printf("declare -x %s\n", sorted_envp[i]);
		free(sorted_envp[i]);
	}
	free(sorted_envp);
}
