#include "minishell.h"

// int	handle_unclosed_quotes(void)
// {
// 	char	*additional_input;

// 	additional_input = readline("quote> ");
// 	if (!additional_input)
// 	{
// 		free(g_data.input);
// 		return (0);
// 	}
// 	if (append_additional_input(additional_input) == -1)
// 	{
// 		free(additional_input);
// 		return (-1);
// 	}
// 	free(additional_input);
// 	return (1);
// }

// int append_additional_input(char *additional_input)
// {
//     g_data.input = ft_strjoin_and_free_first(g_data.input, "\n");
//     if (!g_data.input)
//     {
//         ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
//         return (-1);
//     }
//     g_data.input = ft_strjoin_and_free_first(g_data.input, additional_input);
//     if (!g_data.input)
//     {
//         ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
//         return (-1);
//     }
//     g_data.full_input = ft_strjoin_and_free_first(g_data.full_input, "\n");
//     if (!g_data.full_input)
//     {
//         ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
//         return (-1);
//     }
//     g_data.full_input = ft_strjoin_and_free_first(g_data.full_input, additional_input);
//     if (!g_data.full_input)
//     {
//         ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
//         return (-1);
//     }
//     return (0);
// }



void	process_input(void)
{
	preprocess_input();
}

void	handle_input(void)
{
	while (1)
	{
		// Display prompt and wait for user input
		g_data.input = readline(PROMPT);
		if (!g_data.input)
		{
			// Handle Ctrl-D (EOF): Exit the shell
			write(1, "exit\n", 5);
			break;
		}

		// Check for input length limits
		if (ft_strlen(g_data.input) > MAX_INPUT_LENGTH)
		{
			ft_putstr_fd("minishell: input too long\n", STDERR_FILENO);
			free(g_data.input);
			continue;
		}

		// Duplicate input for processing
		g_data.full_input = ft_strdup(g_data.input);
		if (!g_data.full_input)
		{
			ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
			free(g_data.input);
			continue;
		}

		// Add the full input to history before syntax checking
		if (ft_strlen(g_data.full_input) > 0)
			add_history(g_data.full_input);

		// Check for unclosed quotes
		if (check_unclosed_quotes(g_data.input))
		{
			ft_putstr_fd("minishell: syntax error: unclosed quotes\n", STDERR_FILENO);
			free(g_data.input);
			free(g_data.full_input);
			g_data.input = NULL;
			g_data.full_input = NULL;
			g_data.exit_status = 2; // Set appropriate exit status for syntax error
			continue;
		}

		// Check for trailing pipe
		if (check_trailing_pipe(g_data.input))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
			free(g_data.input);
			free(g_data.full_input);
			g_data.input = NULL;
			g_data.full_input = NULL;
			g_data.exit_status = 2; // Set appropriate exit status for syntax error
			continue;
		}

		// Process the input, tokenize, parse, and prepare for execution
		process_input();

		// Handle redirections if present
		if (handle_redirections() == -1)
		{
			// Clean up if redirection fails
			cleanup();
			break;
		}

		// Execute internal commands
		execute_internal_commands();

		// Execute external commands if no internal command is matched
		if (execute_commands() == -1)
		{
			// Clean up if command execution fails
			cleanup();
			break;
		}

		// Free input buffers after processing
		free(g_data.input);
		g_data.input = NULL;
		free(g_data.full_input);
		g_data.full_input = NULL;
	}

	// Clear the readline history upon exit
	rl_clear_history();
}
