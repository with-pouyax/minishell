#include "minishell.h"

// Check if character is a special variable like $?
int is_special_var(char c)
{
    return (c == '?');
}

// Get variable name length
int get_var_name_len(char *str)
{
    int len = 0;
    if (ft_isdigit(str[len]))
        return (1);
    while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
        len++;
    return (len);
}

// Get environment variable value from envp
char *getenv_from_envp(char *name, t_shell *shell)
{
    int i = 0;
    int name_len = ft_strlen(name);
    while (shell->envp[i])
    {
        if (ft_strncmp(shell->envp[i], name, name_len) == 0
            && shell->envp[i][name_len] == '=')
            return (&shell->envp[i][name_len + 1]);
        i++;
    }
    return (NULL);
}

// Handle quotes in input
char *handle_quotes(char *input, int *i)
{
    char    quote = input[*i];
    char    *content;
    int     start = ++(*i);
    while (input[*i] && input[*i] != quote)
        (*i)++;
    content = ft_substr(input, start, *i - start);
    if (input[*i])
        (*i)++;
    return (content);
}

// Expand variables in input
char *expand_variables(char *input, t_shell *shell, int *var_not_found_flag)
{
    char    *result = ft_strdup("");
    char    *temp;
    int     i = 0;

    while (input[i])
    {
        if (input[i] == '\'')
            temp = handle_quotes(input, &i);
        else if (input[i] == '$')
        {
            i++;
            temp = expand_variable_token(input, &i, shell, var_not_found_flag);
        }
        else if (input[i] == '\"')
            temp = expand_double_quotes(input, &i, shell, var_not_found_flag);
        else
            temp = get_literal_char(input, &i);
        result = ft_strjoin_free(result, temp);
    }
    return (result);
}

