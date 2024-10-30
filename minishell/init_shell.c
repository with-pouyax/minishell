#include "minishell.h"

void	init_shell(void)
{
	g_data.input = NULL;
	g_data.full_input = NULL;
	g_data.exit_status = 0;
	g_data.commands = NULL;
	g_data.error_flag = 0;
	g_data.signal_status = 0;
}
