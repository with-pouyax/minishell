#include "minishell.h"

void    free_envp(void)
{
    if (g_shell.envp)
        free_array(g_shell.envp);
}

char    **copy_envp(void)
{
    int     i;
    int     count;
    char    **env_copy;

    count = 0;
    while (g_shell.envp[count])
        count++;
    env_copy = malloc(sizeof(char *) * (count + 1));
    if (!env_copy)
        return (NULL);
    i = 0;
    while (i < count)
    {
        env_copy[i] = ft_strdup(g_shell.envp[i]);
        i++;
    }
    env_copy[count] = NULL;
    return (env_copy);
}

void    sort_envp(char **envp)
{
    int     count;
    int     i;
    int     j;
    char    *temp;

    count = 0;
    while (envp[count])
        count++;
    i = 0;
    while (i < count - 1)
    {
        j = i + 1;
        while (j < count)
        {
            if (ft_strcmp(envp[i], envp[j]) > 0)
            {
                temp = envp[i];
                envp[i] = envp[j];
                envp[j] = temp;
            }
            j++;
        }
        i++;
    }
}

char    **realloc_envp(char **envp, char *new_var)
{
    int     count;
    int     i;
    char    **new_envp;

    count = 0;
    while (envp[count])
        count++;
    new_envp = malloc(sizeof(char *) * (count + 2));
    if (!new_envp)
        return (NULL);
    i = 0;
    while (i < count)
    {
        new_envp[i] = envp[i];
        i++;
    }
    new_envp[count] = new_var;
    new_envp[count + 1] = NULL;
    free(envp);
    return (new_envp);
}

int     set_env_var(char *arg)
{
    char    *name;
    char    *value;
    int     i;

    i = 0;
    while (arg[i] && arg[i] != '=')
        i++;
    name = ft_substr(arg, 0, i);
    if (arg[i] == '=')
        value = ft_strdup(&arg[i + 1]);
    else
        value = ft_strdup("");
    if (update_env(name, value) != 0)
    {
        free(name);
        free(value);
        return (1);
    }
    free(name);
    free(value);
    return (0);
}
