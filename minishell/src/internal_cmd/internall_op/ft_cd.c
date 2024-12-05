#include "../internal_commands.h"

/*
	expanded_path = ft_strdup(home);                // Path is just "~"
	expanded_path = ft_strjoin(home, &path[1]);     // Path starts with "~/"
	in handle_exec_error:
	ft_putstr_fd("minishell: cd: no such file or directory: ", STDERR_FILENO)
	-> Handle cases like "~user" (optional)

*/
int	handle_tilde_path(t_shell_data *shell, char *path)
{
	char	*home;
	char	*expanded_path;

	home = getenv_from_envp(shell, "HOME");
	if (!home)
		return (handle_no_home(shell));
	if (path[1] == '\0')
		expanded_path = ft_strdup(home);
	else if (path[1] == '/')
		expanded_path = ft_strjoin(home, &path[1]);
	else
	{
		write_error(path, "\n");
		shell->exit_status = 1;
		return (1);
	}
	if (!expanded_path)
		return (handle_memory_error(shell));
	if (chdir(expanded_path) != 0)
		return (handle_chdir_error(shell, expanded_path));
	free(expanded_path);
	shell->exit_status = 0;
	return (0);
}

int	change_to_home(t_shell_data *shell)
{
	char	*home;

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
	shell->exit_status = 0;
	return (0);
}

/*
	token = cmd->token_list->next;         // Skip the command token
	path = token->value;                   // Argument provided
	if (chdir(path) != 0)                  // Regular path

*/
int	ft_cd(t_shell_data *shell, t_command *cmd)
{
	t_token	*token;
	char	*path;

	token = cmd->token_list->next;
	if (!token)
		return (change_to_home(shell));
	else
	{
		path = token->value;
		if (path[0] == '~')
			return (handle_tilde_path(shell, path));
		else
		{
			if (chdir(path) != 0)
			{
				ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
				perror(path);
				shell->exit_status = 1;
				return (1);
			}
		}
	}
	shell->exit_status = 0;
	return (0);
}
