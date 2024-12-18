#include "../minishell.h"
#include <sys/ioctl.h>


t_signal g_signal = {0, 0};



void child_sigint_handler(int sig) 
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    g_signal.signal_status = 1;
}

void	sigint_hd_handler(int sig)
{
	(void)sig;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	g_signal.signal_status = 1;
}

void	sigint_handler(int sig)
{
	(void)sig;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 0); 
	g_signal.signal_status = 1;
}



void	sigquit_handler(int sig)
{
	(void)sig;
}

void	setup_signal_handlers(int type)
{
	if (type == 0)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (type == 1)
	{
		signal(SIGINT, sigint_hd_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (type == 2)
	{
		signal(SIGINT, child_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (type == 3)
	{
		signal(SIGINT, child_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
}