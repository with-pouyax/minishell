/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by yourname          #+#    #+#             */
/*   Updated: 2024/10/21 15:53:11 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void free_commands(void)
{
    t_command   *cmd;
    t_token     *token;
    t_token     *next_token;

    while (g_data.commands)
    {
        cmd = g_data.commands;
        g_data.commands = cmd->next;
        token = cmd->token_list;
        while (token)
        {
            next_token = token->next;
            free(token->value);
            free(token);
            token = next_token;
        }
        free(cmd->command_string);
        free(cmd);
    }
}


