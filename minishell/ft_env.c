// ft_env.c
#include "internal_commands.h"

int	ft_env(t_command *cmd)
{
	int	i;

	(void)cmd; // Unused parameter

	i = 0;
	while (g_data.envp[i])
	{
		printf("%s\n", g_data.envp[i]);
		i++;
	}

	g_data.exit_status = 0;
	return (0);
}
