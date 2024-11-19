// ft_cd.c
#include "internal_commands.h"

int	ft_cd(t_shell_data *shell, t_command *cmd)
{
	t_token	*token;
	char	*path;
	char	*home;
	char	*expanded_path;

	token = cmd->token_list->next; // Skip the command token

	if (!token)
	{
		// No argument provided, change to HOME
		home = getenv_from_envp(shell, "HOME");
		if (!home)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			shell->exit_status = 1;
			return (1);
		}
		if (chdir(home) != 0)
		{
			perror("minishell: cd");
			shell->exit_status = 1;
			return (1);
		}
	}
	else
	{
		// Argument provided
		path = token->value;

		if (path[0] == '~')
		{
			home = getenv_from_envp(shell, "HOME");
			if (!home)
			{
				ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
				shell->exit_status = 1;
				return (1);
			}
			if (path[1] == '\0')
			{
				// Path is just "~"
				expanded_path = ft_strdup(home);
			}
			else if (path[1] == '/')
			{
				// Path starts with "~/"
				expanded_path = ft_strjoin(home, &path[1]);
			}
			else
			{
				// Handle cases like "~user" (optional)
				ft_putstr_fd("minishell: cd: no such file or directory: ", STDERR_FILENO);
				ft_putstr_fd(path, STDERR_FILENO);
				ft_putchar_fd('\n', STDERR_FILENO);
				shell->exit_status = 1;
				return (1);
			}

			if (!expanded_path)
			{
				ft_putstr_fd("minishell: cd: memory allocation error\n", STDERR_FILENO);
				shell->exit_status = 1;
				return (1);
			}

			if (chdir(expanded_path) != 0)
			{
				perror("minishell: cd");
				free(expanded_path);
				shell->exit_status = 1;
				return (1);
			}
			free(expanded_path);
		}
		else
		{
			// Regular path
			if (chdir(path) != 0)
			{
				perror("minishell: cd");
				shell->exit_status = 1;
				return (1);
			}
		}
	}

	shell->exit_status = 0;
	return (0);
}
