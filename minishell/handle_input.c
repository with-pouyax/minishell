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
	if (g_data.full_input && ft_strlen(g_data.full_input) > 0) // If the full_input is not NULL and its
		add_history(g_data.full_input); // Add the full_input to the history
	else if (g_data.input && ft_strlen(g_data.input) > 0) // If the full_input is NULL and the input is not NULL
		add_history(g_data.input); 
	preprocess_input(); //Preprocess the input before tokenizing it
	g_data.commands = NULL;
	tokenize_input();
	if (!g_data.commands)
		return ;
	expand_variables_in_tokens();
	parse_tokens();
	print_commands();
	free_commands();
}

void	handle_input(void)
{
	int	ret;

	while (1)
	{
		g_data.input = readline(PROMPT); // Read the input from the user and store it in the global data structure g_data.input
		if (!g_data.input) // If the input is NULL, break out of the loop because the user has pressed Ctrl-D
			break ;
		g_data.full_input = ft_strdup(g_data.input); // copy the input to the full_input
		if (check_unclosed_quotes(g_data.input)) // Check if there are any unclosed quotes in the input
		{
			ret = handle_unclosed_quotes(); // Prompt the user for additional input
			if (ret <= 0) // If the return value is less than or equal to 0, that means the user has pressed Ctrl-D or there was an error
			{
				cleanup();
				break ;
			}
		}
		process_input(); // Process the input
		free(g_data.input);
		g_data.input = NULL;
		free(g_data.full_input);
		g_data.full_input = NULL;
	}
	rl_clear_history();
}
