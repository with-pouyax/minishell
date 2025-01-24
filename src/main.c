/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 11:11:27 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:33:34 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*****************************************************************************/
//                              🚀 main 🚀
/*****************************************************************************/
// 🎯 Purpose  : we start the programm here
/*****************************************************************************/
// 🔹 Parameters:
//     🏷  argv  -> array of strings that represent the command line arguments
//     🏷  envp  -> array of strings that
// 🔄 Returns   :  exit code.
/*****************************************************************************/
// 💡 Notes:
//     1- we make sure we have only one argument.
//     2- we check if the input is interactive or not using isatty() function.
//     3- we zero out the shell struct.
//     4- we copy the envp to the envp in the shell struct.
//     5- we initialize the shell struct.
//     6- we setup the signal handlers with 0 (means we are in the main
//        process).
//	   7- we handle the input.
//     8- we clean up the shell struct.
//     9- we return the exit status from the shell struct.
/*****************************************************************************/

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
