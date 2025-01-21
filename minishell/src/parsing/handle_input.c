/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:37:13 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/21 15:37:37 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*****************************************************************************/
//                            🚀 handle_ctrl_d 🚀
/*****************************************************************************/
// 🎯 Purpose  :    handle control + d
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- we call cleanup() to free all the resources and exit with the proper
//        exit status.
/******************************************************************************/

void	handle_ctrl_d(t_shell_data *shell)
{
	cleanup(shell);
	exit(shell->exit_status);
}
/*****************************************************************************/
// 🎯 Purpose  :  purpose: process and execute the commands
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we preprocess (meaning we tokenize and parse) the input using
//        preprocess_input().
//        a- if there is an error in the preprocess_input() we return 1.
//     2- if there are commands in the shell we execute them using execution().
//     3- if there are no commands (usually because of syntax errors or empty
//        input) we set the exit status to 2.
//     4- after we are passing and executing the commands we free the shell
//        resources using free_shell_resources().
/******************************************************************************/

int	process_and_execute_commands(t_shell_data *shell)
{
	if (preprocess_input(shell) != 0)
		return (1);
	// printf("\nDebug: shell->cmds_nb: %d\n\n", shell->cmds_nb);
	// printf("\nDebug: shell->pipe_nb: %d\n\n", shell->pipe_nb);
	//print_commands(shell);
	if (shell->commands)
	{
		//printf("Debug: Starting execution()---------------------------------------\n");
		execution(shell);
		//printf("Debug: Finished execution()-------------------------------------\n");
	}
	else
		shell->exit_status = 2;
	free_shell_resources(shell);
	return (0);
}

/*****************************************************************************/
//                      🚀 check_and_handle_syntax_errors 🚀
/*****************************************************************************/
// 🎯 Purpose  :  Check for some syntax errors
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- using check_unclosed_quotes() we check if there are any unclosed
//        quotes.
//        a- if there are unclosed quotes we print an error message and
//           cleanup the shell and return 1.
//     2- using check_leading_pipe() we check if there is pipe at the beginning
//        of the input.
//        a- if there is a pipe at the beginning we print an error message and
//           cleanup the shell and return 1.
//     3- using check_trailing_pipe() we check if there is a pipe at the end of
//        the input.
//        a- if there is a pipe at the end we print an error message and
//           cleanup the shell and return 1.
//     4- if everything is fine we return 0.
/******************************************************************************/

int	check_and_handle_syntax_errors(t_shell_data *shell)
{
	if (check_unclosed_quotes(shell->input))
	{
		check_syntax_error(shell, "minishell: syntax error: unclosed quotes\n");
		cleanup(shell);
		return (1);
	}
	else if (check_leading_pipe(shell->input))
	{
		check_syntax_error(shell, "syntax error near unexpected token `|'\n");
		cleanup(shell);
		return (1);
	}
	else if (check_trailing_pipe(shell->input))
	{
		check_syntax_error(shell, "minishell: syntax error near unexpected \
		token `|'\n");
		cleanup(shell);
		return (1);
	}
	return (0);
}

/*****************************************************************************/
//                        🚀 check_input_length 🚀
/*****************************************************************************/
// 🎯 Purpose  :  check if the input is not too long
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the input is longer than MAX_INPUT_LENGTH we print an error
//		message and free the input and return 1.
//     2- if everything is fine we return 0.
/******************************************************************************/

int	check_input_length(t_shell_data *shell)
{
	if (ft_strlen(shell->input) > MAX_INPUT_LENGTH)
	{
		ft_putstr_fd("minishell: input too long\n", STDERR_FILENO);
		free(shell->input);
		return (1);
	}
	return (0);
}

/*****************************************************************************/
//                        🚀 add_to_history_if_needed 🚀
/*****************************************************************************/
// 🎯 Purpose  :  add to history if input is not empty
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the input is not empty we add it to the history.
/******************************************************************************/

void	add_to_history_if_needed(t_shell_data *shell)
{
	if (ft_strlen(shell->full_input) > 0)
		add_history(shell->full_input);
}
/*****************************************************************************/
//                        🚀 handle_allocation 🚀
/*****************************************************************************/
// 🎯 Purpose  :  allocate resources needed for the shell
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we allocate memory dynamically in heap for shell->full_input.
//     2- if there is an error allocating the resources we print an error
//        message and return 1.
//     3- if everything is fine we return 0.
/******************************************************************************/

int	handle_allocation(t_shell_data *shell)
{
	shell->full_input = ft_strdup(shell->input);
	if (!shell->full_input)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}
/*****************************************************************************/
//                            🚀 read_input 🚀
/*****************************************************************************/
// 🎯 Purpose  :    read input from the user
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we read the input using readline() and store the return value in
//        shell->input, we are prompted with PROMPT.
//     2- if shell->input is NULL (means user pressed ctrl+d) we call
//        handle_ctrl_d()  to handle the ctrl+d signal and return 1.
//     3- if everything is fine we return 0.
/******************************************************************************/
int	read_input(t_shell_data *shell)
{
	shell->input = readline(PROMPT);
	if (!shell->input)
	{
		handle_ctrl_d(shell);
		return (1);
	}
	return (0);
}
/*****************************************************************************/
//                        🚀 validate_input_length 🚀
/*****************************************************************************/
// 🎯 Purpose  :  allocate resources needed for the shell
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- if there is an error allocating the resources we return 1.
//     2- if everything is fine we return 0.
/******************************************************************************/

int	allocate_resources(t_shell_data *shell)
{
	if (handle_allocation(shell))
	{
		return (1);
	}
	return (0);
}

/*****************************************************************************/
//                        🚀 validate_input_length 🚀
/*****************************************************************************/
// 🎯 Purpose  :  check if the input is not too long
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- using check_input_length() we check if the input is not too long.
//        a- if it is too long we call cleanup() and cleanup the all the
//           resources and return 1.
//	 2- if everything is fine we return 0.
/******************************************************************************/

int	validate_input_length(t_shell_data *shell)
{
	if (check_input_length(shell))
	{
		cleanup(shell);
		return (1);
	}
	return (0);
}
/*****************************************************************************/
//                        🚀 handle_empty_input 🚀
/*****************************************************************************/
// 🎯 Purpose  :  check if the function is empty
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- we free the shell->input and set it to NULL.
//     2- we set the signal status to 0 because we go back to the main process.
/******************************************************************************/

void	handle_empty_input(t_shell_data *shell)
{
	free(shell->input);
	shell->input = NULL;
	g_signal.signal_status = 0;
}

/*****************************************************************************/
//                        🚀 is_input_empty 🚀
/*****************************************************************************/
// 🎯 Purpose  :  check if the function is empty
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  1 if it is empty.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we check if the input is NULL or the first character is '\0'.
/******************************************************************************/

int	is_input_empty(t_shell_data *shell)
{
	return (shell->input == NULL || shell->input[0] == '\0');
}

/*****************************************************************************/
//                        🚀 handle_execution_flow 🚀
/*****************************************************************************/
// 🎯 Purpose  :  main function to handle user input
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we set the signal status to 0.
//     2- using validate_input_length() we check if the input is not too long.
//        a- if it is too long we return 1.
//     3- using allocate_resources() we allocate the resources needed for the
//        shell.
//        a- if there is an error allocating the resources we return 1.
//     4- using add_to_history_if_needed() we add the input to the history if
//        it is not empty and we have no errors.
//     5- using check_and_handle_syntax_errors() we check if there are any
//        syntax errors in the input.
//        a- if there are syntax errors we return 1.
//     6- using process_and_execute_commands() we process and execute the
//        commands.
//        a- if there is an error in the process and execute commands
//           I- if the error flag is 2 or 3 we return 0.
//           II- if the error flag is not 2 or 3 we return 1.
//	 7- if everything is fine we return 1 because as you remember in
//        handle_input() we store the return value inside running, and it keeps
//        prompting the user as long as running is 1.
/******************************************************************************/

int	handle_execution_flow(t_shell_data *shell)
{
	g_signal.signal_status = 0;
	if (validate_input_length(shell))
		return (1);
	if (allocate_resources(shell))
		return (1);
	add_to_history_if_needed(shell);
	if (check_and_handle_syntax_errors(shell))
		return (1);
	if (process_and_execute_commands(shell) != 0)
	{
		if (shell->error_flag == 2 || shell->error_flag == 3)
			return (0);
		return (1);
	}
	return (1);
}
/*****************************************************************************/
//                        🚀 handle_user_input 🚀
/*****************************************************************************/
// 🎯 Purpose  :  main function to handle user input
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we check if the input is empty using is_input_empty()
//        a- if it is empty we call handle_empty_input() and return 1.
//     2- if it is not empty we call handle_execution_flow() to handle the
//        user input and return the return value of it.
/******************************************************************************/
int	handle_user_input(t_shell_data *shell)
{
	if (is_input_empty(shell))
	{
		handle_empty_input(shell);
	}
	else
	{
		return (handle_execution_flow(shell));
	}
	return (1);
}
/*****************************************************************************/
//                            🚀 handle_read_error 🚀
/*****************************************************************************/
// 🎯 Purpose  :    print error
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  void
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we print the error message using ft_putstr_fd() to STDERR_FILENO,
//        which is standard error.
/******************************************************************************/
int	handle_read_error(void)
{
	ft_putstr_fd("Error: Failed to read input\n", STDERR_FILENO);
	return (0);
}
/*****************************************************************************/
//                            🚀 handle_input 🚀
/*****************************************************************************/
// 🎯 Purpose  :    handle the input from the user
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we set the running flag to 1.
//     2- we start a while loop that runs until user presses ctrl+d.
//        a- we read the input using read_input() and store the return value
//           of it in status.
//        b- if status is -1 means we have an error in reading the input or
//           user pressed ctrl+d so we call handle_read_error() and set running
//           to 0 to exit the loop.
//        c- if status is not -1 we call handle_user_input() to handle the user
//           input and store the return value in running.
//        d- we setup the signal handlers with 0 (means we are in the main
//           process).
//     3- now that we are out of the loop we clear the history and return 0.
/******************************************************************************/
int	handle_input(t_shell_data *shell)
{
	int	running;
	int	status;

	running = 1;
	while (running)
	{
		status = read_input(shell);
		if (status == -1)
			running = handle_read_error();
		else
		{
			running = handle_user_input(shell);
			setup_signal_handlers(0);
		}
	}
	rl_clear_history();
	return (0);
}
