/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by yourname          #+#    #+#             */
/*   Updated: 2024/10/21 15:52:31 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



void parse_tokens(void)
{
    t_command   *cmd;
    t_token     *token;

    cmd = g_data.commands;
    while (cmd)
    {
        token = cmd->token_list;
        while (token)
        {
            if (token->index == 0 && !token->is_operator)
            {
                token->is_command = 1;
                if (is_internal_command(token->value))
                    token->is_int = 1;
            }
            else if (ft_strncmp(token->value, "-", 1) == 0)
                token->is_flag = 1;
            token = token->next;
        }
        cmd = cmd->next;
    }
}


