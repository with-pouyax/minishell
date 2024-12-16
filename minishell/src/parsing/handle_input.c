#include "../minishell.h"

void handle_ctrl_d(t_shell_data *shell)
{
    cleanup(shell); // Free all allocated resources
    exit(shell->exit_status);        // Exit the shell
}

int	process_and_execute_commands(t_shell_data *shell)
{
    if (preprocess_input(shell) != 0)              // Preprocess input
	{
		return 1;
	}
	// printf("\nDebug: shell->cmds_nb: %d\n\n", shell->cmds_nb);
	// printf("\nDebug: shell->pipe_nb: %d\n\n", shell->pipe_nb);
	//print_commands(shell);
	if (shell->commands)
	{
		//printf("Debug: Starting execution()---------------------------------------\n");
		execution(shell); // Currently commented out
		//printf("Debug: Finished execution()-------------------------------------\n");
	}
	else
		shell->exit_status = 2;
	free_shell_resources(shell);
	return (0);
}

int	check_and_handle_syntax_errors(t_shell_data *shell)
{
	if (check_unclosed_quotes(shell->input))             // Check for unclosed quotes
	{
		check_syntax_error(shell, "minishell: syntax error: unclosed quotes\n");
		cleanup(shell);
		return (1);
	}
	else if (check_leading_pipe(shell->input)) 		     // Check for leading pipe
	{
		check_syntax_error(shell, "minishell: syntax error near unexpected token `|'\n");
		cleanup(shell);
		return (1);
	}
	else if (check_trailing_pipe(shell->input))          // Check for trailing pipe
	{
		check_syntax_error(shell, "minishell: syntax error near unexpected token `|'\n");
		cleanup(shell);
		return (1);
	}
	return (0);
}

int check_input_length(t_shell_data *shell)
{
    if (ft_strlen(shell->input) > MAX_INPUT_LENGTH)
    {
        ft_putstr_fd("minishell: input too long\n", STDERR_FILENO);
        free(shell->input);
        return (1);
    }
    return (0);
}

void	add_to_history_if_needed(t_shell_data *shell)
{
	if (ft_strlen(shell->full_input) > 0)
		add_history(shell->full_input);
}

int handle_allocation(t_shell_data *shell)
{
    shell->full_input = ft_strdup(shell->input);
    if (!shell->full_input)
    {
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
        return (1);
    }
    return (0);
}

int read_input(t_shell_data *shell)
{
    shell->input = readline(PROMPT); 			// Read user input from terminal, if input is NULL, user pressed Ctrl-D
    if (!shell->input)
    {
        handle_ctrl_d(shell);
        return (1); // Break the loop
    }
    return (0);
}

int	allocate_resources(t_shell_data *shell)
{
	if (handle_allocation(shell))                   // Allocate memory for full_input
	{
		//cleanup(shell);
		return (1);
	}
	return (0);
}

int	validate_input_length(t_shell_data *shell)
{
	if (check_input_length(shell)) 
	{
		cleanup(shell);
		return (1);
	}
	return (0);
}
//----------------------------------------------------------

void	handle_empty_input(t_shell_data *shell)
{
	free(shell->input);
	shell->input = NULL;
	g_signal.signal_status = 0;                  // Reset signal status
}

int	is_input_empty(t_shell_data *shell)
{
	return (shell->input == NULL || shell->input[0] == '\0'); 
}


int handle_user_input(t_shell_data *shell)
{	
	if (is_input_empty(shell))                  					// If input is empty, do nothing
		handle_empty_input(shell);
	else 									   						// if input is not empty
	{
		g_signal.signal_status = 0; 		   						// Reset signal status
		if (!validate_input_length(shell))     						//check if input length is less than MAX_INPUT_LENGTH
		{
			if (!allocate_resources(shell))    						// Allocate resources for full_input field in shell 
			{
				add_to_history_if_needed(shell);   					// Add input to history if it is not empty
				if (!check_and_handle_syntax_errors(shell)) 		// Check for syntax errors
				{
					if (process_and_execute_commands(shell) != 0)  	// Process and execute commands
						return (1);
				}
			}
			else 
				return (0);
		}
	}
	return (1); // Continue running
}

int	handle_read_error(void)
{
	ft_putstr_fd("Error: Failed to read input\n", STDERR_FILENO); 							// print error message to stderr
	return (0); 
}

int	handle_input(t_shell_data *shell)
{
	int	running;
	int	status;

	running = 1;
	while (running)
	{
		status = read_input(shell); 							// Read user input from terminal, if successful, status = 0, if user pressed Ctrl-D, status = 1
		if (status == -1) 										// status == -1 means read error
			running = handle_read_error();						// handle read error
		else													// if read is successful
			running = handle_user_input(shell);					// handle user input
	}
	rl_clear_history();
	return (0);
}