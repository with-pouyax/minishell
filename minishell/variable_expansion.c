/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyaximac <pouyaximac@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by yourname          #+#    #+#             */
/*   Updated: 2024/10/21 21:04:28 by pouyaximac       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Function to expand variables in tokens
void expand_variables_in_tokens(t_shell *shell)
{
    t_command   *cmd;
    t_token     *token;
    char        *expanded_value;
    int         var_not_found_flag;

    cmd = shell->commands;
    while (cmd)
    {
        token = cmd->token_list;
        while (token)
        {
            if (!token->is_operator)
            {
                var_not_found_flag = 0;
                expanded_value = expand_variables_in_token(token->value, shell, &var_not_found_flag);
                free(token->value);
                token->value = expanded_value;
                token->var_not_found = var_not_found_flag;
            }
            token = token->next;
        }
        cmd = cmd->next;
    }
}

// Function to expand variables in a single token
char *expand_variables_in_token(char *input, t_shell *shell, int *var_not_found_flag)
{
    char *result = ft_strdup("");
    char *temp;
    int i = 0;
    int in_single_quote = 0;
    int in_double_quote = 0;

    while (input[i])
    {
        if (input[i] == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            temp = get_literal_char(input, &i); // Include the quote
            result = ft_strjoin_free(result, temp);
        }
        else if (input[i] == '\"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            temp = get_literal_char(input, &i); // Include the quote
            result = ft_strjoin_free(result, temp);
        }
        else if (input[i] == '$' && !in_single_quote)
        {
            i++;
            temp = expand_variable_token(input, &i, shell, var_not_found_flag);
            result = ft_strjoin_free(result, temp);
        }
        else
        {
            temp = get_literal_char(input, &i);
            result = ft_strjoin_free(result, temp);
        }
    }
    return (result);
}

// Function to expand a variable token
char *expand_variable_token(char *input, int *i, t_shell *shell, int *var_not_found_flag)
{
    char    *var_name;
    char    *var_value;
    int     var_len;

    if (input[*i] == '?')
    {
        (*i)++;
        var_value = ft_itoa(shell->exit_status);
    }
    else if (ft_isdigit(input[*i]))
    {
        var_name = ft_substr(input, (*i)++, 1);
        var_value = ft_strdup("");
        free(var_name);
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
            *var_not_found_flag = 1; // Set the flag when variable is not found
        }
        *i += var_len;
        free(var_name);
    }
    return (var_value);
}

// Function to get a literal character
char *get_literal_char(char *input, int *i)
{
    char *str = ft_substr(input, *i, 1);
    (*i)++;
    return (str);
}

// Function to get variable name length
int get_var_name_len(char *str)
{
    int len = 0;

    while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
        len++;

    return len;
}

// Function to get environment variable value from envp
char *getenv_from_envp(char *name, t_shell *shell)
{
    int i = 0;
    int len = ft_strlen(name);

    while (shell->envp[i])
    {
        if (ft_strncmp(shell->envp[i], name, len) == 0 && shell->envp[i][len] == '=')
            return (shell->envp[i] + len + 1);
        i++;
    }
    return NULL;
}
