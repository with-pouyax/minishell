#include "../internal_commands.h"

int handle_cd_minus(t_shell_data *shell)
{
    char *current_dir;

    if (shell->prev_dir == NULL)
    {
        ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
        shell->exit_status = 1;
        return (1);
    }
    current_dir = getcwd(NULL, 0);
    if (chdir(shell->prev_dir) != 0)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        perror(shell->prev_dir);
        shell->exit_status = 1;
        return (1);
    }
    printf("%s\n", shell->prev_dir);
    if (current_dir)
    {
        free(shell->prev_dir);
        shell->prev_dir = current_dir;
    }
    shell->exit_status = 0;
    return (0);
}

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


static int validate_path(const char *path)
{
    if (strstr(path, "..$"))
    {
        ft_putstr_fd((char *)path, STDERR_FILENO);  // Cast to char * to avoid const qualifier issues
        ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
        return (1);
    }
    return (0);
}

static int update_prev_dir(t_shell_data *shell, char *current_dir)
{
    if (current_dir)
    {
        free(shell->prev_dir);
        shell->prev_dir = current_dir;
    }
    return (0);
}

int ft_cd(t_shell_data *shell, t_command *cmd)
{
    t_token *token;
    char *path;
    char *current_dir;

    token = cmd->token_list->next;
    if (!token)
        return change_to_home(shell);
    if (token->next != NULL)
        return (ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO), shell->exit_status = 1, 1);
    path = token->value;
    if (validate_path(path))
        return (shell->exit_status = 1);
    if (strcmp(path, "-") == 0)
        return handle_cd_minus(shell);
    if (path[0] == '~')
        return handle_tilde_path(shell, path);
    current_dir = getcwd(NULL, 0);
    if (chdir(path) != 0)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        perror(path);
        return (shell->exit_status = 1);
    }
    return update_prev_dir(shell, current_dir);
}
