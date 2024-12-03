#include "../minishell.h"

void	preprocess_input(t_shell_data *shell)
{
	shell->commands = NULL;
	split_cmd_tokenize(shell);
	if (!shell->commands)
		return ;
	if (shell->error_flag)
	{
		free_commands(shell);
		shell->commands = NULL;
		return ;
	}
	parse_tokens(shell);
	if (validate_operators(shell))
		return ;
	}
	// print_commands(shell);
	// free_commands(shell);
}
