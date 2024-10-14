#include "minishell.h"

void    init_shell(char **envp)
{
    int i;
    int env_count;

    // Count environment variables
    for (env_count = 0; envp[env_count]; env_count++)
        ;

    // Allocate memory for g_envp
    g_envp = malloc(sizeof(char *) * (env_count + 1));
    if (!g_envp)
    {
        perror("minishell: init_shell");
        exit(1);
    }

    // Copy environment variables
    for (i = 0; i < env_count; i++)
        g_envp[i] = ft_strdup(envp[i]);
    g_envp[env_count] = NULL;

    // Initialize other variables if necessary
}

void    cleanup_shell(void)
{
    if (g_envp)
        free_array(g_envp);
    // Free other allocated resources if necessary
}
