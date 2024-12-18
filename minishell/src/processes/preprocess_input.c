#include "../minishell.h"

int	preprocess_input(t_shell_data *shell)
{
	shell->commands = NULL;									// Initialize commands to NULL
	if (split_cmd_tokenize(shell) != 0)						//[x] if there are pipes, we split the commands separated by pipes
		return 1;
	if (!shell->commands)
		return 1;
	if (shell->error_flag)
	{
		free_commands(shell);
		shell->commands = NULL;
		return 1;
	}
	parse_tokens(shell);									//[x] 
	if (validate_operators(shell))
		return 1;
	return 0;
}
