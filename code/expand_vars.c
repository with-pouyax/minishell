#include "minishell.h"

char    *get_env_value(char *var)
{
    int     i;
    size_t  len;
    char    *env_var;

    len = ft_strlen(var);
    i = 0;
    while (g_shell.envp[i])
    {
        if (ft_strncmp(g_shell.envp[i], var, len) == 0
            && g_shell.envp[i][len] == '=')
        {
            env_var = ft_strdup(&g_shell.envp[i][len + 1]);
            return (env_var);
        }
        i++;
    }
    return (ft_strdup(""));
}

char    *expand_env_vars(char *input)
{
    int     i;
    int     j;
    char    *result;
    char    *temp;
    char    var_name[BUFFER_SIZE];

    result = ft_strdup("");
    i = 0;
    while (input[i])
    {
        if (input[i] == '$' && input[i + 1]
            && (ft_isalnum(input[i + 1]) || input[i + 1] == '_'))
        {
            i++;
            j = 0;
            while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
                var_name[j++] = input[i++];
            var_name[j] = '\0';
            temp = get_env_value(var_name);
            result = ft_strjoin_free(result, temp);
        }
        else if (input[i] == '$' && input[i + 1] == '?')
        {
            i += 2;
            temp = ft_itoa(g_shell.last_exit_status);
            result = ft_strjoin_free(result, temp);
        }
        else
        {
            temp = ft_substr(input, i, 1);
            result = ft_strjoin_free(result, temp);
            i++;
        }
    }
    return (result);
}
