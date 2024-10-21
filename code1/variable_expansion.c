/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by yourname          #+#    #+#             */
/*   Updated: 2024/10/21 16:22:00 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char     *expand_variable(char *var_name, t_shell *shell, int *var_not_found_flag)
{
        char    *var_value;

        if (ft_strcmp(var_name, "?") == 0)
                var_value = ft_itoa(shell->exit_status);
        else
        {
                var_value = getenv_from_envp(var_name, shell);
                if (var_value)
                        var_value = ft_strdup(var_value);
                else
                {
                        var_value = ft_strdup("");
                        *var_not_found_flag = 1;
                }
        }
        return (var_value);
}

static char     *handle_dollar(char *input, int *i, t_shell *shell, int *var_not_found_flag)
{
        char    *var_name;
        char    *var_value;
        int     start;

        start = *i;
        if (input[*i] == '?')
        {
                (*i)++;
                var_name = ft_strdup("?");
        }
        else if (ft_isdigit(input[*i]))
        {
                var_name = ft_substr(input, (*i)++, 1);
        }
        else
        {
                while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
                        (*i)++;
                var_name = ft_substr(input, start, *i - start);
        }
        var_value = expand_variable(var_name, shell, var_not_found_flag);
        free(var_name);
        return (var_value);
}

char    *expand_variables_in_token(char *input, t_shell *shell, int *var_not_found_flag)
{
        char    *result;
        char    *temp;
        int     i;

        result = ft_strdup("");
        i = 0;
        while (input[i])
        {
                if (input[i] == '$')
                {
                        i++;
                        temp = handle_dollar(input, &i, shell, var_not_found_flag);
                }
                else
                {
                        temp = ft_substr(input, i, 1);
                        i++;
                }
                result = ft_strjoin_free(result, temp);
        }
        return (result);
}

void    expand_variables_in_tokens(t_shell *shell)
{
        t_command       *cmd;
        t_token         *token;
        char            *expanded_value;
        int             var_not_found_flag;

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
