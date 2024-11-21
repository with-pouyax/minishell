#include "minishell.h"





/*
The isatty function is used to check if a file descriptor refers to a terminal.
first : Makes sure that the terminal is linked to the STDIN
*/


int main(int argc, char **argv, char **envp)
{
    t_shell_data shell;
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
        // printf("before handle input");
	handle_input(&shell);
    printf("after handle input");

    // printf("before execution");
    // execution();
    //  printf("after execution");

	cleanup(&shell);
	return (0);
}




