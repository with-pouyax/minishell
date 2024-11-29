// signal_handling.c
#include "../minishell.h"

volatile sig_atomic_t g_signal_status = 0;

// In signal_handling.c
void sigint_handler(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
    g_signal_status = 1;
}
void sigquit_handler(int sig)
{
    (void)sig;
}
// In signal_handling.c
void setup_signal_handlers(void)
{
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
}