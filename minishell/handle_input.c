#include "minishell.h"

void	process_input(t_shell_data *shell)
{
	expand_variables_in_input(shell); //ok 
	preprocess_input(shell); //ok
}

void	handle_input(t_shell_data *shell)
{
	while (1)
	{
		shell->input = readline(PROMPT); // Read input from user and print prompt and store in g_data.input
		if (!shell->input) // If input is NULL (Ctrl-D) , exit the shell
		{
			write(1, "exit\n", 5);
			break;
		}
		if (ft_strlen(shell->input) > MAX_INPUT_LENGTH)
		{
			ft_putstr_fd("minishell: input too long\n", STDERR_FILENO);
			free(shell->input);
			continue;
		}
		shell->full_input = ft_strdup(shell->input);
		if (!shell->full_input)
		{
			ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
			free(shell->input);
			continue;
		}
		if (ft_strlen(shell->full_input) > 0) // If input is not empty, add to history
			add_history(shell->full_input);
		if (check_unclosed_quotes(shell->input)) // Check for unclosed quotes
		{
			ft_putstr_fd("minishell: syntax error: unclosed quotes\n", STDERR_FILENO);
			free(shell->input);
			free(shell->full_input);
			shell->input = NULL;
			shell->full_input = NULL;
			shell->exit_status = 2;
			continue;
		}
		if (check_trailing_pipe(shell->input)) // Check for trailing pipe, trailing means at the end of the input
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
			free(shell->input);
			free(shell->full_input);
			shell->input = NULL;
			shell->full_input = NULL;
			shell->exit_status = 2;
			continue;
		}
		process_input(shell); // Expand variables and preprocess input 
		if (execute_internal_commands(shell) == -1)
		{
			//cleanup(shell);
			break;
		}

		//free_commands(shell); // ##important
		if (execute_commands() == -1)
		{
			cleanup(shell);
			break;
		}

		free_commands(shell); // ##important
		

		free(shell->input);
		shell->input = NULL;
		free(shell->full_input);
		shell->full_input = NULL;
	}
	rl_clear_history();
}

