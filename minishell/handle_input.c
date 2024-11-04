#include "minishell.h"

int	handle_unclosed_quotes(void)
{
	char	*additional_input;

	additional_input = readline("quote> ");
	if (!additional_input)
	{
		free(g_data.input);
		return (0);
	}
	if (append_additional_input(additional_input) == -1)
	{
		free(additional_input);
		return (-1);
	}
	free(additional_input);
	return (1);
}

int	append_additional_input(char *additional_input)
{
	g_data.input = ft_strjoin_and_free_first(g_data.input, additional_input);
	if (!g_data.input)
	{
		ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
		return (-1);
	}
	g_data.full_input = ft_strjoin_and_free_first(
			g_data.full_input, "\nquote> ");
	if (!g_data.full_input)
	{
		ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
		return (-1);
	}
	g_data.full_input = ft_strjoin_and_free_first(
			g_data.full_input, additional_input);
	if (!g_data.full_input)
	{
		ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
		return (-1);
	}
	return (0);
}

void	process_input(void)
{
	preprocess_input();
}

void	handle_input(void)
{
	int	ret;

	while (1)
	{
		g_data.input = readline(PROMPT); // Read input from the user and store it in .input
		if (!g_data.input) // if ctrl-D is pressed
			break ;
		g_data.full_input = ft_strdup(g_data.input); // Copy the input to .full_input for history
		if (check_unclosed_quotes(g_data.input)) // Check if there are unclosed quotes
		{
			ret = handle_unclosed_quotes(); // If there are unclosed quotes, prompt the user for more input
			if (ret <= 0)
			{
				cleanup();
				break ;
			}
		}
		process_input();
		free(g_data.input);
		g_data.input = NULL;
		free(g_data.full_input);
		g_data.full_input = NULL;
	}
	rl_clear_history();
}