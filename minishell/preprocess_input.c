#include "minishell.h"

void	preprocess_input(void)
{
	char	*expanded_input;
	int		var_not_found_flag;

	var_not_found_flag = 0;
	expanded_input = expand_variables_in_token(g_data.input,
			&var_not_found_flag); // Expand variables in the input
	if (!expanded_input) // If the expansion failed
	{
		ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
		free(g_data.input);
		free(g_data.full_input);
		exit(EXIT_FAILURE);
	}
	free(g_data.input);
	g_data.input = expanded_input; // Store the expanded input in .input
	g_data.commands = NULL;
	tokenize_input();



	if (!g_data.commands)
    {
        printf("preprocess_input: No commands parsed.\n");
        return ;
    }
    else
    {
        printf("preprocess_input: Commands have been parsed and stored.\n");
    }




	if (!g_data.commands)
		return ;

	// Check if input is from history and set `is_recalled` flag
	if (g_data.full_input && is_recalled_command(g_data.full_input))
		set_recalled_flag(g_data.commands);

	process_heredocs(); // Process heredocs after tokenization

	if (g_data.error_flag)
	{
		//free_commands();   lol
		g_data.commands = NULL;
		return ;
	}
	// Move add_history here to include heredoc inputs
	if (g_data.full_input && ft_strlen(g_data.full_input) > 0)
		add_history(g_data.full_input);
	else if (g_data.input && ft_strlen(g_data.input) > 0)
		add_history(g_data.input);

	//expand_variables_in_tokens();
	parse_tokens();
	print_commands();
	//free_commands();   lol
}

