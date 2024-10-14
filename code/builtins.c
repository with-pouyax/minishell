#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_command *cmd)
{
	if (ft_strcmp(cmd->cmd, "cd") == 0)
		return (builtin_cd(cmd->args));
	else if (ft_strcmp(cmd->cmd, "echo") == 0)
		return (builtin_echo(cmd->args));
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
		return (builtin_pwd());
	else if (ft_strcmp(cmd->cmd, "export") == 0)
		return (builtin_export(cmd->args));
	else if (ft_strcmp(cmd->cmd, "unset") == 0)
		return (builtin_unset(cmd->args));
	else if (ft_strcmp(cmd->cmd, "env") == 0)
		return (builtin_env());
	else if (ft_strcmp(cmd->cmd, "exit") == 0)
		return (builtin_exit(cmd->args));
	return (1);
}

int	builtin_cd(char **args)
{
	char	*path;

	if (args[1] == NULL)
		path = getenv("HOME");
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (args[1] && ft_strcmp(args[1], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

int	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		return (0);
	}
	else
	{
		perror("minishell: pwd");
		return (1);
	}
}

int	builtin_export(char **args)
{
	int	i;

	if (args[1] == NULL)
		print_sorted_env();
	else
	{
		i = 1;
		while (args[i])
		{
			if (set_env_var(args[i]) != 0)
				return (1);
			i++;
		}
	}
	return (0);
}

int	builtin_unset(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		unset_env_var(args[i]);
		i++;
	}
	return (0);
}

int	builtin_env(void)
{
	int	i;

	i = 0;
	while (g_envp[i])
	{
		ft_putendl_fd(g_envp[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}

int	builtin_exit(char **args)
{
	int	exit_status;

	exit_status = 0;
	if (args[1] != NULL)
	{
		if (ft_isnumber(args[1]))
			exit_status = ft_atoi(args[1]);
		else
		{
			ft_putendl_fd("minishell: exit: numeric argument required", STDERR_FILENO);
			exit_status = 255;
		}
	}
	ft_putendl_fd("exit", STDERR_FILENO);
	cleanup_shell();
	exit(exit_status);
	return (0);
}

