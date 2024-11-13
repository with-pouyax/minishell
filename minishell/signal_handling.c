// signal_handling.c
#include "minishell.h"


void	sigint_handler(int sig)
{
	(void)sig;
	if (g_data.in_child_process)
	{
		// If in child process, default behavior
		write(1, "\n", 1);
	}
	else
	{
		// If in parent process, write newline and re-display prompt
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sigquit_handler(int sig)
{
	(void)sig;
	// Do nothing
}

void	setup_signal_handlers(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
}
