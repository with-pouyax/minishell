#include "../minishell.h"

void	process_input(t_shell_data *shell)
{
	expand_variables_in_input(shell);
	preprocess_input(shell);                      // Preprocess input (tokenize, parse)
}