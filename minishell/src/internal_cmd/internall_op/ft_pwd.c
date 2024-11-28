#include "../internal_commands.h"


int	ft_pwd(t_shell_data *shell,t_command *cmd)
{
	char	cwd[PATH_MAX];

	(void)cmd;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		shell->exit_status = 0;
	}
	else
	{
		perror("minishell: pwd");
		shell->exit_status = 1;
	}
	return (0);
}
