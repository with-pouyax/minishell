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



// Function to free the commands list
void	free_commands(t_command *cmd_list)
{
	t_command	*cmd;
	t_token		*token;
	t_token		*next_token;

	while (cmd_list)
	{
		cmd = cmd_list;
		cmd_list = cmd_list->next;
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

