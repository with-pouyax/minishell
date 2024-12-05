#include "../minishell.h"

int	preprocess_input(t_shell_data *shell)
{
	shell->commands = NULL;
	if (split_cmd_tokenize(shell) != 0)
		return 1;
	if (!shell->commands)
		return 1;
	if (shell->error_flag)
	{
		free_commands(shell);
		shell->commands = NULL;
		return 1;
	}
	parse_tokens(shell);
	if (validate_operators(shell))
		return 1;
	return 0;
}
