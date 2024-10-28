#include "minishell.h"

// Expand variable token
char *expand_variable_token(char *input, int *i, t_shell *shell, int *var_not_found_flag)
{
    char    *var_name;
    char    *var_value;
    int     var_len;

    if (is_special_var(input[*i]))
    {
        var_name = ft_substr(input, (*i)++, 1);
        if (var_name[0] == '?')
            var_value = ft_itoa(shell->exit_status);
        else
            var_value = ft_strdup("");
    }
    else
    {
        var_len = get_var_name_len(&input[*i]);
        var_name = ft_substr(input, *i, var_len);
        var_value = getenv_from_envp(var_name, shell);
        if (var_value)
            var_value = ft_strdup(var_value);
        else
        {
            var_value = ft_strdup("");
            *var_not_found_flag = 1;  // Set the flag when variable is not found
        }
        *i += var_len;
    }
    free(var_name);
    return (var_value);
}


// Expand double quotes
char *expand_double_quotes(char *input, int *i, t_shell *shell, int *var_not_found_flag)
{
    char    *result = ft_strdup("");
    char    *temp;
    (*i)++;
    while (input[*i] && input[*i] != '\"')
    {
        if (input[*i] == '$')
        {
            (*i)++;
            temp = expand_variable_token(input, i, shell, var_not_found_flag);
        }
        else
            temp = get_literal_char(input, i);
        result = ft_strjoin_free(result, temp);
    }
    if (input[*i] == '\"')
        (*i)++;
    return (result);
}


// Get literal character
char *get_literal_char(char *input, int *i)
{
    char *str;

    str = ft_substr(input, *i, 1);
    (*i)++;
    return (str);
}


