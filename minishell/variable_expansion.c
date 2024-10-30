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

char *get_literal_char(void)
{
    char *str;

    str = ft_substr(g_data.expansion_input, g_data.i, 1);
    if (!str)
        return (NULL);
    g_data.i++;
    return (str);
}


char *expand_variable_token(void)
{
    char *var_name;
    char *var_value;
    int var_len;

    if (g_data.expansion_input[g_data.i] == '?')
    {
        g_data.i++;
        var_value = ft_itoa(g_data.exit_status);
    }
    else
    {
        var_len = get_var_name_len(&g_data.expansion_input[g_data.i]);
        var_name = ft_substr(g_data.expansion_input, g_data.i, var_len);
        if (!var_name)
            return (NULL);
        var_value = getenv_from_envp(var_name);
        if (var_value)
            var_value = ft_strdup(var_value);
        else
        {
            var_value = ft_strdup("");
            g_data.var_not_found_flag = 1;
        }
        g_data.i += var_len;
        free(var_name);
    }
    return (var_value);
}


char *expand_variables_in_token(void)
{
    char *result;
    char *temp;
    int in_single_quote;
    int in_double_quote;

    result = ft_strdup("");
    if (!result)
        return (NULL);
    g_data.i = 0;
    in_single_quote = 0;
    in_double_quote = 0;
    while (g_data.expansion_input[g_data.i])
    {
        if (g_data.expansion_input[g_data.i] == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        else if (g_data.expansion_input[g_data.i] == '\"' && !in_single_quote)
            in_double_quote = !in_double_quote;
        if (g_data.expansion_input[g_data.i] == '$' && !in_single_quote)
        {
            g_data.i++;
            temp = expand_variable_token();
        }
        else
        {
            temp = get_literal_char();
        }
        if (!temp)
        {
            free(result);
            return (NULL);
        }
        result = ft_strjoin_free_both(result, temp);
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

    cmd = g_data.commands;
    while (cmd)
    {
        token = cmd->token_list;
        while (token)
        {
            if (!token->is_operator)
            {
                g_data.var_not_found_flag = 0;
                g_data.expansion_input = token->value;
                expanded_value = expand_variables_in_token();
                if (!expanded_value)
                {
                    // Handle error (e.g., free resources, set error flag)
                    return;
                }
                free(token->value);
                token->value = expanded_value;
                token->var_not_found = g_data.var_not_found_flag;
            }
            token = token->next;
        }
        cmd = cmd->next;
    }
}

