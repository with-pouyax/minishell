// signal_handling.c
#include "minishell.h"


// void	sigint_handler(int sig)
// {
// 	(void)sig;
// 	if (g_data.in_child_process) // Check if in child process (executing a command)
// 	{
// 		write(1, "\n", 1); // Write newline
// 	}
// 	else // Not in child process
// 	{
// 		write(1, "\n", 1); // Write newline
// 		rl_replace_line("", 0); // Clear the input line
// 		rl_on_new_line(); // Move cursor to new line
// 		rl_redisplay(); // Redisplay the prompt
// 	}
// }

// void	sigquit_handler(int sig)
// {
// 	(void)sig;
// }

// void	setup_signal_handlers(void)
// {
// 	signal(SIGINT, sigint_handler); // Handle SIGINT (Ctrl-C)
// 	signal(SIGQUIT, sigquit_handler); // Handle SIGQUIT (Ctrl-\)
// }
