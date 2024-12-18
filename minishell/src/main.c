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
	if (argc > 1)
		return (ft_putstr_fd("no arguments are allowed\n", \
		STDERR_FILENO), EXIT_FAILURE);
	if (!isatty(STDIN_FILENO) && !isatty(STDOUT_FILENO)) 						//isatty() function is used to check if a file descriptor refers to a terminal.  
		shell.interactive_mode = 0;
	else
		shell.interactive_mode = 1;
	ft_bzero(&shell, sizeof(t_shell_data));										//here with ft_bzero we are initializing the shell structure to 0 
	shell.envp = copy_envp(envp);												//copying the envp to the shell.envp
	if (!shell.envp)
	{
		ft_putstr_fd("malloc failed\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	init_shell(&shell);  														//initialize the shell
	setup_signal_handlers(0);													//setup the signal handlers
	handle_input(&shell);													    //handle the input
	cleanup(&shell);														    //clean up the shell
	return (shell.exit_status);
}





/*
do it first
run :
ulimit -n 1048576
ulimit -u 62899
in terinal then run ./minishell

*/
// some point that we have to considered later:
/*
- check  ft_pwd() : there is error in this function
- check the exit_code
- heredoc
- do we need to return 0 in "execute_internal_commands()"?
*/

// expand_variables_in_token --> in this function while loop has not i++!!!
