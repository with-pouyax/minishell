#include "minishell.h"

void    init_shell(char **envp)
{
    int i;
    int env_count;

    env_count = 0;
    while (envp[env_count])
        env_count++;
    g_shell.envp = malloc(sizeof(char *) * (env_count + 1));
    if (!g_shell.envp)
    {
        perror("minishell: init_shell");
        exit(1);
    }
    i = 0;
    while (i < env_count)
    {
        g_shell.envp[i] = ft_strdup(envp[i]);
        i++;
    }
    g_shell.envp[env_count] = NULL;
    g_shell.last_exit_status = 0;
    g_shell.cmd_list = NULL;
}

void    cleanup_shell(void)
{
    if (g_shell.envp)
        free_array(g_shell.envp);
}
