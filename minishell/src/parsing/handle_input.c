#include "../minishell.h"

void handle_ctrl_d(t_shell_data *shell)
{
    cleanup(shell);
    exit(shell->exit_status);
}

int	process_and_execute_commands(t_shell_data *shell)
{
    if (preprocess_input(shell) != 0)
	{
		return 1;
	}
	// printf("\nDebug: shell->cmds_nb: %d\n\n", shell->cmds_nb);
	// printf("\nDebug: shell->pipe_nb: %d\n\n", shell->pipe_nb);
	//print_commands(shell);
	if (shell->commands)
	{
		printf("Debug: Starting execution()---------------------------------------\n");
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
    shell->input = readline(PROMPT);
    if (!shell->input)
    {
        handle_ctrl_d(shell);
        return (1);
    }
    return (0);
}

int	allocate_resources(t_shell_data *shell)
{
	if (handle_allocation(shell))
	{
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


void	handle_empty_input(t_shell_data *shell)
{
	free(shell->input);
	shell->input = NULL;
	g_signal.signal_status = 0;
}

int	is_input_empty(t_shell_data *shell)
{
	return (shell->input == NULL || shell->input[0] == '\0'); 
}


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

int	handle_read_error(void)
{
	ft_putstr_fd("Error: Failed to read input\n", STDERR_FILENO);
	return (0); 
}

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
