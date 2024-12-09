#include "../minishell.h"

volatile sig_atomic_t	g_signal_status = 0;

int	ft_putstr(char *str)
{
	return (write(1, str, ft_strlen(str)));
}

void	sigint_handler(int sig)
{
	(void)sig;
	ft_putstr("\n");
}

void	sigquit_handler(int sig)
{
	(void)sig;
	printf("Quit: 3\n");
	kill(0, SIGCHLD);

}


void	setup_signal_handlers(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
}
