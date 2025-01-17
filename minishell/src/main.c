#include "minishell.h"

/******************************************************************************/  
/*                              🚀 main 🚀                                         
/******************************************************************************/
/* 🎯 Purpose  : 
/******************************************************************************/

/* 🔹 Parameters:                                                             
/*     🏷  int a  ➝  First integer                                           
/*     🏷  int b  ➝  Second integer                                          

/* 🔄 Returns   :  exit code.

/******************************************************************************/

/* 💡 Notes:                                                                  
/*     1- we make sure we have only one argument.                             
/*     - 

/******************************************************************************/



int	main(int argc, char **argv, char **envp)
{
	t_shell_data	shell;

	(void)argc;
	(void)argv;
	if (argc > 1)
		return (ft_putstr_fd("no arguments are allowed\n", \
		STDERR_FILENO), EXIT_FAILURE);
	if (!isatty(STDIN_FILENO) && !isatty(STDOUT_FILENO)) 
		shell.interactive_mode = 0;
	else
		shell.interactive_mode = 1;
	ft_bzero(&shell, sizeof(t_shell_data));
	shell.envp = copy_envp(envp);
	if (!shell.envp)
	{
		ft_putstr_fd("malloc failed\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	init_shell(&shell);
	setup_signal_handlers(0);
	handle_input(&shell);
	cleanup(&shell);
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
