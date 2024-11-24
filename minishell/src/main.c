#include "minishell.h"

/*
The isatty function is used to check if a file descriptor refers to a terminal.
first : Makes sure that the terminal is linked to the STDIN
*/

int	main(int argc, char **argv, char **envp)
{
	t_shell_data	shell;

	(void)argc;
	(void)argv;
	if (!isatty(STDIN_FILENO))
		exit(EXIT_FAILURE);
	ft_bzero(&shell, sizeof(t_shell_data));
	shell.envp = copy_envp(envp); // okey
	if (!shell.envp)
	{
		ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	init_shell(&shell);
	// setup_signal_handlers(); // Set up signal handlers
	handle_input(&shell);
	printf("after handle input");
	cleanup(&shell);
	return (0);
}
// some point that we have to considered later:
/*
- check  ft_pwd() : there is error in this function
- check the exit_code
- heredoc
- do we need to return 0 in "execute_internal_commands()"?
*/
