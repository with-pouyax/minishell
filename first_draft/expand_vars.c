#include "minishell.h"




char *expand_variables_in_word(char *word)
{
    int     i;
    int     start;
    char    *result;
    char    *temp;
    char    *var_name;
    char    *var_value;

    result = ft_strdup("");
    i = 0;
    while (word[i])
    {
        if (word[i] == '$' && word[i + 1] && (ft_isalnum(word[i + 1]) || word[i + 1] == '_'))
        {
            i++;
            start = i;
            while (word[i] && (ft_isalnum(word[i]) || word[i] == '_'))
                i++;
            var_name = ft_substr(word, start, i - start);
            var_value = getenv(var_name);
            free(var_name);
            if (var_value)
            {
                temp = ft_strjoin_free(result, ft_strdup(var_value));
                result = temp;
            }
        }
        else
        {
            temp = ft_strjoin_char(result, word[i]);
            free(result);
            result = temp;
            i++;
        }
    }
    return (result);
}


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

char *expand_env_vars(char *input)
{
    char *result;
    char *status_str;

    result = ft_strdup(input);
    // Check if $? is in the input
    if (ft_strstr(input, "$?"))
    {
        status_str = ft_itoa(g_shell.last_exit_status); // Convert last exit status to string
        result = ft_strreplace(result, "$?", status_str); // Replace $? with status_str
        free(status_str);
    }
    return result;
}
