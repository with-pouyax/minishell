// ft_pwd.c
#include "internal_commands.h"
#include <limits.h> // For PATH_MAX

int	ft_pwd(t_command *cmd)
{
	char	cwd[PATH_MAX];

	(void)cmd; // Unused parameter

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		g_data.exit_status = 0;
	}
	else
	{
		perror("minishell: pwd");
		g_data.exit_status = 1;
	}

	return (0);
}
