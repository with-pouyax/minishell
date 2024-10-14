#include "minishell.h"

void	exit_shell(int status)
{
	cleanup_shell();
	exit(status);
}
