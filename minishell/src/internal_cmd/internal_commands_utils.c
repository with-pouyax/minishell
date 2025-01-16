#include "internal_commands.h"

int	is_allowed_char(char c)
{
	return (ft_isalnum(c)
		|| c == '_'
		|| c == '-'
		|| c == '.'
		|| c == '/'
		|| c == '~'
		|| c == '$'
		|| c == '='
		|| c == ':');
}

int is_valid_identifier(const char *str, int allow_equals) {
    int i = 0;

    if (!str || (!ft_isalpha(str[i]) && str[i] != '_'))
        return 0;
    while (str[i]) {
        if (!ft_isalnum(str[i]) && str[i] != '_') 
		{
            if (allow_equals && str[i] == '=')
                return 1;
            return 0;
        }
        i++;
    }
    if (!allow_equals && ft_strchr(str, '='))
        return 0;
    return 1;
}

void	add_to_env(t_shell_data *shell, const char *str)
{
    int     i;
    char    *key;
    char    *new_var;
    int     len;
	int		j;

    len = 0;
	j = 0;
    while (str[len] && str[len] != '=')
        len++;
    key = ft_substr(str, 0, len);
    if (!key)
    {
        ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
        shell->exit_status = 1;
        return ;
    }
    i = 0;
    while (shell->envp[i])
    {
        if (ft_strncmp(shell->envp[i], key, len) == 0 && \
		shell->envp[i][len] == '=')
        {
            free(shell->envp[i]);
            shell->envp[i] = ft_strdup(str);
            if (!shell->envp[i])
            {
                ft_putstr_fd("minishell: export: allocation error\n", \
				STDERR_FILENO);
                shell->exit_status = 1;
            }
            free(key);
            return ;
        }
        i++;
    }
    free(key);
    new_var = ft_strdup(str);
    if (!new_var)
    {
        ft_putstr_fd("minishell: export: allocation error\n", \
		STDERR_FILENO);
        shell->exit_status = 1;
        return ;
    }
    char **new_envp = malloc(sizeof(char *) * (i + 2));
    if (!new_envp)
    {
        ft_putstr_fd("minishell: export: allocation error\n", \
		STDERR_FILENO);
        free(new_var);
        shell->exit_status = 1;
        return ;
    }
    while (j < i)
    {
		new_envp[j] = shell->envp[j];
		j++;
	}
    new_envp[i] = new_var;
    new_envp[i + 1] = NULL;
    free(shell->envp);
    shell->envp = new_envp;
    shell->exit_status = 0;
}

void	remove_from_env(t_shell_data *shell,const char *name)
{
	int	i;
	int	j;
	int	len;
	char	**new_envp;

	len = ft_strlen(name);
	i = 0;
	while (shell->envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 1));
	if (!new_envp)
	{
		ft_putstr_fd("minishell: unset: allocation error\n", STDERR_FILENO);
		shell->exit_status = 1;
		return;
	}
	i = 0;
	j = 0;
	while (shell->envp[i])
	{
		if (!(ft_strncmp(shell->envp[i], name, len) == 0 &&
			  (shell->envp[i][len] == '=' || shell->envp[i][len] == '\0')))
			new_envp[j++] = shell->envp[i];
		else
			free(shell->envp[i]);
		i++;
	}
	new_envp[j] = NULL;
	free(shell->envp);
	shell->envp = new_envp;
}

/* Print sorted environment variables */
void	print_sorted_env(t_shell_data *shell)
{
	int		i, j;
	int		env_size;
	char	**sorted_envp;
	char	*temp;

	// Count environment variables
	env_size = 0;
	while (shell->envp[env_size])
		env_size++;

	// Duplicate envp
	sorted_envp = malloc(sizeof(char *) * (env_size + 1));
	if (!sorted_envp)
	{
		ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
		shell->exit_status = 1;
		return;
	}
	i = 0;
	while(i < env_size)
	{
		sorted_envp[i] = ft_strdup(shell->envp[i]);
		i++;
	}
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
