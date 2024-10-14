#include "minishell.h"
#include <readline/readline.h> // Add this include
#include <readline/history.h>  // Add this include

void    signal_setup(void)
{
    struct sigaction    sa_int;
    struct sigaction    sa_quit;

    sa_int.sa_handler = &sigint_handler;
    sa_int.sa_flags = SA_RESTART;
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);

    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}

void    sigint_handler(int signum)
{
    (void)signum;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}
