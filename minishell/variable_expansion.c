#include "minishell.h"

char *getenv_from_envp(char *name)
{
    int i;
    int len;

    i = 0;
    len = ft_strlen(name);
    while (g_data.envp[i])
    {
        if (ft_strncmp(g_data.envp[i], name, len) == 0 && g_data.envp[i][len] == '=')
            return (g_data.envp[i] + len + 1);
        i++;
    }
    return (NULL);
}

int get_var_name_len(char *str)
{
    int len;

    len = 0;
    while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
        len++;
    return (len);
}

char *get_literal_char(char *input, int *i)
{
    char *str;

    str = ft_substr(input, *i, 1);
    (*i)++;
    return (str);
}

char *expand_variable_token(char *input, int *i, int *var_not_found_flag)
{
    char *var_name;
    char *var_value;
    int var_len;

    if (input[*i] == '?')
    {
        (*i)++;
        var_value = ft_itoa(g_data.exit_status);
    }
    else
    {
        var_len = get_var_name_len(&input[*i]);
        var_name = ft_substr(input, *i, var_len);
        var_value = getenv_from_envp(var_name);
        if (var_value)
            var_value = ft_strdup(var_value);
        else
        {
            var_value = ft_strdup("");
            *var_not_found_flag = 1;
        }
        *i += var_len;
        free(var_name);
    }
    return (var_value);
}

char *expand_variables_in_token(char *input, int *var_not_found_flag)
{
    char *result;
    char *temp;
    int i;
    int in_single_quote;
    int in_double_quote;

    result = ft_strdup("");
    if (!result)
        return (NULL);
    i = 0;
    in_single_quote = 0;
    in_double_quote = 0;
    while (input[i])
    {
        if (input[i] == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        else if (input[i] == '\"' && !in_single_quote)
            in_double_quote = !in_double_quote;
        if (input[i] == '$' && !in_single_quote)
        {
            i++;
            temp = expand_variable_token(input, &i, var_not_found_flag);
        }
        else
        {
            temp = get_literal_char(input, &i);
        }
        result = ft_strjoin_free(result, temp);
        if (!result)
            return (NULL);
    }
    return (result);
}

void expand_variables_in_tokens(void)
{
    t_command *cmd;
    t_token *token;
    char *expanded_value;
    int var_not_found_flag;

    cmd = g_data.commands;
    while (cmd)
    {
        token = cmd->token_list;
        while (token)
        {
            if (!token->is_operator)
            {
                var_not_found_flag = 0;
                expanded_value = expand_variables_in_token(token->value,
                                                           &var_not_found_flag);
                free(token->value);
                token->value = expanded_value;
                token->var_not_found = var_not_found_flag;
            }
            token = token->next;
        }
        cmd = cmd->next;
    }
}
