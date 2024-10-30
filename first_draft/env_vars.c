#include "minishell.h"

void    unset_env_var(char *name)
{
    int     i;
    int     j;
    char    **new_envp;
    size_t  name_len;

    name_len = ft_strlen(name);
    i = 0;
    j = 0;
    while (g_shell.envp[i])
        i++;
    new_envp = malloc(sizeof(char *) * i);
    if (!new_envp)
        return ;
    i = 0;
    while (g_shell.envp[i])
    {
        if (ft_strncmp(g_shell.envp[i], name, name_len) == 0
            && g_shell.envp[i][name_len] == '=')
            free(g_shell.envp[i]);
        else
        {
            new_envp[j] = g_shell.envp[i];
            j++;
        }
        i++;
    }
    new_envp[j] = NULL;
    free(g_shell.envp);
    g_shell.envp = new_envp;
}

int     update_env(char *name, char *value)
{
    int     i;
    char    *new_var;
    size_t  name_len;

    new_var = ft_strjoin3(name, "=", value);
    if (!new_var)
        return (1);
    name_len = ft_strlen(name);
    i = 0;
    while (g_shell.envp[i])
    {
        if (ft_strncmp(g_shell.envp[i], name, name_len) == 0
            && g_shell.envp[i][name_len] == '=')
        {
            free(g_shell.envp[i]);
            g_shell.envp[i] = new_var;
            return (0);
        }
        i++;
    }
    g_shell.envp = realloc_envp(g_shell.envp, new_var);
    if (!g_shell.envp)
    {
        free(new_var);
        return (1);
    }
    return (0);
}

void    print_sorted_env(void)
{
    char    **sorted_envp;
    int     i;

    sorted_envp = copy_envp();
    if (!sorted_envp)
    {
        perror("minishell: print_sorted_env");
        return ;
    }
    sort_envp(sorted_envp);
    i = 0;
    while (sorted_envp[i])
    {
        ft_putstr_fd("declare -x ", STDOUT_FILENO);
        ft_putendl_fd(sorted_envp[i], STDOUT_FILENO);
        free(sorted_envp[i]);
        i++;
    }
    free(sorted_envp);
}
