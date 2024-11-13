#include "minishell.h"

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
		{
			write(1, "exit\n", 5);
			break;
		}
		if (ft_strlen(g_data.input) > MAX_INPUT_LENGTH)
		{
			ft_putstr_fd("minishell: input too long\n", STDERR_FILENO);
			free(g_data.input);
			continue;
		}
		g_data.full_input = ft_strdup(g_data.input);
		if (!g_data.full_input)
		{
			ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
			free(g_data.input);
			continue;
		}
		if (ft_strlen(g_data.full_input) > 0)
			add_history(g_data.full_input);
		if (check_unclosed_quotes(g_data.input))
		{
			ft_putstr_fd("minishell: syntax error: unclosed quotes\n", STDERR_FILENO);
			free(g_data.input);
			free(g_data.full_input);
			g_data.input = NULL;
			g_data.full_input = NULL;
			g_data.exit_status = 2;
			continue;
		}
		if (check_trailing_pipe(g_data.input))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
			free(g_data.input);
			free(g_data.full_input);
			g_data.input = NULL;
			g_data.full_input = NULL;
			g_data.exit_status = 2;
			continue;
		}

		// Expand variables in input before processing
		expand_variables_in_input();

		process_input();
		if (execute_internal_commands() == -1)
		{
			//cleanup();
			break;
		}
		if (execute_commands() == -1)
		{
			cleanup();
			break;
		}
		free(g_data.input);
		g_data.input = NULL;
		free(g_data.full_input);
		g_data.full_input = NULL;
	}
	rl_clear_history();
}

