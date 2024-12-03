#include "../minishell.h"

void handle_ctrl_d(t_shell_data *shell)
{
    write(1, "exit\n", 5);
    cleanup(shell); // Free all allocated resources
    exit(0);        // Exit the shell
}

void	process_and_execute_commands(t_shell_data *shell)
{
    preprocess_input(shell);
	// printf("\nDebug: shell->cmds_nb: %d\n\n", shell->cmds_nb);
	// printf("\nDebug: shell->pipe_nb: %d\n\n", shell->pipe_nb);
	if (shell->commands)
	{
		// printf("Debug: Starting execution()---------------------------------------\n");
		execution(shell); // Currently commented out
		// printf("Debug: Finished execution()-------------------------------------\n");
	}
	else
		shell->exit_status = 2;
	free_shell_resources(shell);
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
		check_syntax_error(shell, "minishell: syntax error near unexpected token `|'\n");
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
        free(shell->input);
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
        return (1); // Break the loop
    }
    return (0);
}

int	allocate_resources(t_shell_data *shell)
{
	if (handle_allocation(shell))
	{
		cleanup(shell);
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
	g_signal_status = 0;
}

void	handle_input(t_shell_data *shell)
{
	while (1)
	{
		if (read_input(shell))
			break;
		if (shell->input == NULL || shell->input[0] == '\0')
		    handle_empty_input(shell);
		else
		{
			g_signal_status = 0;
			if (!validate_input_length(shell))
			{
				if (!allocate_resources(shell))
				{
					add_to_history_if_needed(shell);
					if (!check_and_handle_syntax_errors(shell))
						process_and_execute_commands(shell);
				}
			}
		}
	}
	rl_clear_history();
}

