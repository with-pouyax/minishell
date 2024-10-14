#include "minishell.h"

int		g_last_exit_status = 0;
char	**g_envp = NULL;
t_command *g_cmd_list = NULL;

void	shell_loop(void)
{
	char	*input;

	load_history();
	while (1)
	{
		signal_setup();
		input = readline("minishell$ ");
		if (input == NULL)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (*input != '\0')
		{
			add_to_history(input);
			input = expand_env_vars(input);
			if (parse_input(input) == 0)
			{
				execute_commands(g_cmd_list);
				free_command_list(g_cmd_list);
				g_cmd_list = NULL;
			}
			else
				g_last_exit_status = 2;
		}
		free(input);
	}
	save_history();
	cleanup_shell();
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	init_shell(envp);
	shell_loop();
	return (g_last_exit_status);
}
