/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:46:18 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/30 15:29:54 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Main function that manages execution of external commands.
** Splits into smaller functions to respect Norminette line limits.
*/
void	execute_external_commands(t_shell_data *shell, t_command *cmds)
{
	int		token_count;
	char	*cmd_path;
	char	**arr_token;

	arr_token = allocate_and_fill_argv1(shell, cmds, &token_count);
	if (!arr_token[0])
	{
		free(arr_token);
		return ;
	}
	cmd_path = resolve_command_path(shell, cmds, arr_token);
	if (!cmd_path)
		return ;
	fork_and_exec1(shell, cmd_path, arr_token, token_count);
	free(cmd_path);
	free_argv(arr_token, token_count);
}

void	forking(t_shell_data *shell, t_command *cmds)
{
	setup_signal_handlers(2);
	if (cmds->token_list && cmds->token_list->is_int)
	{
		execute_internal_commands(shell, cmds);
	}
	else
	{
		execute_external_commands(shell, cmds);
	}
}
