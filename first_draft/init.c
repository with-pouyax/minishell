#include "minishell.h"

void    init_shell(char **envp)
{
    int i;  
    int env_count; 

    env_count = 0;
    while (envp[env_count]) // Count the number of environment variables
        env_count++; // Count the number of environment variables
    /* we need number of environment variables to allocate memory for g_shell.envp, in envp we have all the environment variables */
    g_shell.envp = malloc(sizeof(char *) * (env_count + 1));
    if (!g_shell.envp)
    {
        perror("minishell: init_shell");
        exit(1);
    }
    i = 0;
    while (i < env_count) // in a loop we copy all the environment variables to g_shell.envp because we need to modify them later
    {
        g_shell.envp[i] = ft_strdup(envp[i]);
        i++;
    }
    g_shell.envp[env_count] = NULL; // NULL terminate the array of environment variables
    g_shell.last_exit_status = 0; // Initialize the last exit status to 0
    g_shell.cmd_list = NULL; // Initialize the command list to NULL, cmd_list is a linked list that stores all the commands entered by the user
}

void    cleanup_shell(void)
{
    if (g_shell.envp)
        free_array(g_shell.envp);
}
