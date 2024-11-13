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
		g_data.input = readline(PROMPT);
		if (!g_data.input)
			break ;

		// Check input length
		if (ft_strlen(g_data.input) > MAX_INPUT_LENGTH)
		{
			ft_putstr_fd("minishell: input too long\n", STDERR_FILENO);
			free(g_data.input);
			continue ;
		}

		// Duplicate input
		g_data.full_input = ft_strdup(g_data.input);
		if (!g_data.full_input)
		{
			ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
			free(g_data.input);
			continue ;
		}

		// Add to history before syntax checking
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
			continue ;
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
			continue ;
		}

		// Process the input
		process_input();

		/* 
		 * +-------------------------------+
		 * |        NEXT PHASE        |
		 * +-------------------------------+
		 */
		
		// Handle redirections
		if (handle_redirections() == -1)
		{
			cleanup();
			break ;
		}

		// Execute internal commands
		execute_internal_commands();

		// Execute external commands
		if (execute_commands() == -1)
		{
			cleanup();
			break ;
		}
		
		/* 
		 * +-------------------------------+
		 * |        END EXECUTION          |
		 * +-------------------------------+
		 */

		// Free input buffers
		free(g_data.input);
		g_data.input = NULL;
		free(g_data.full_input);
		g_data.full_input = NULL;
	}
	rl_clear_history();

	//printf("handle_input: Exiting input loop.\n"); // ##debug print
}
