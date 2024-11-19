#include "minishell.h"

void	init_shell(t_shell_data *shell)
{
	shell->input = NULL;
	shell->full_input = NULL;
	shell->exit_status = 0;
	shell->commands = NULL;
	shell->error_flag = 0;
	shell->signal_status = 0;
}
