#include "minishell.h"

t_shell_data g_data = {0};

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    g_data.envp = envp;
    init_shell();
    handle_input();
    return (0);
}
