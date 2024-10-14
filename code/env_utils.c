#include "minishell.h"

void    free_envp(void)
{
    if (g_envp)
        free_array(g_envp);
}

char    **copy_envp(void)
{
    int     i;
    int     count;
    char    **env_copy;

    for (count = 0; g_envp[count]; count++)
        ;
    env_copy = malloc(sizeof(char *) * (count + 1));
    if (!env_copy)
        return NULL;
    for (i = 0; i < count; i++)
        env_copy[i] = ft_strdup(g_envp[i]);
    env_copy[count] = NULL;
    return env_copy;
}

int     env_var_cmp(const void *a, const void *b)
{
    return ft_strcmp(*(const char **)a, *(const char **)b);
}

void    sort_envp(char **envp)
{
    int count = 0;
    while (envp[count])
        count++;
    qsort(envp, count, sizeof(char *), env_var_cmp);
}

char    **realloc_envp(char **envp, char *new_var)
{
    int     count = 0;
    char    **new_envp;

    while (envp[count])
        count++;
    new_envp = malloc(sizeof(char *) * (count + 2));
    if (!new_envp)
        return NULL;
    for (int i = 0; i < count; i++)
        new_envp[i] = envp[i];
    new_envp[count] = new_var;
    new_envp[count + 1] = NULL;
    free(envp);
    return new_envp;
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

void    print_sorted_env(void)
{
    char    **sorted_envp;
    int     i;

    sorted_envp = copy_envp();
    if (!sorted_envp)
    {
        perror("minishell: print_sorted_env");
        return;
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
void    unset_env_var(char *name)
{
    int     i;
    int     j;
    char    **new_envp;
    size_t  name_len;

    name_len = ft_strlen(name);
    for (i = 0; g_envp[i]; i++)
        ;
    new_envp = malloc(sizeof(char *) * i); // One less than original
    if (!new_envp)
        return;
    i = 0;
    j = 0;
    while (g_envp[i])
    {
        if (ft_strncmp(g_envp[i], name, name_len) == 0 && g_envp[i][name_len] == '=')
            free(g_envp[i]); // Skip this variable
        else
            new_envp[j++] = g_envp[i];
        i++;
    }
    new_envp[j] = NULL;
    free(g_envp);
    g_envp = new_envp;
}

int update_env(char *name, char *value)
{
    int     i;
    char    *new_var;
    size_t  name_len;

    new_var = ft_strjoin3(name, "=", value);
    if (!new_var)
        return (1);
    name_len = ft_strlen(name);
    for (i = 0; g_envp[i]; i++)
    {
        if (ft_strncmp(g_envp[i], name, name_len) == 0 && g_envp[i][name_len] == '=')
        {
            free(g_envp[i]);
            g_envp[i] = new_var;
            return (0);
        }
    }
    // Variable not found; add it
    g_envp = realloc_envp(g_envp, new_var);
    if (!g_envp)
    {
        free(new_var);
        return (1);
    }
    return (0);
}


