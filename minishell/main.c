#include "minishell.h"

t_shell_data	g_data = {0};

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	g_data.envp = envp; // Save the environment variables in the global data
	init_shell(); // Initialize the shell
	handle_input();
	return (0);
}
