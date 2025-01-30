
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:46:18 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:21:34 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

void	in_child(t_shell_data *shell, char	*cmd_path, char	**arr_token)
{
	exec_external_child(shell, cmd_path, arr_token);
	ft_exit_child(shell);
}

void	execute_external_commands(t_shell_data *shell, t_command *cmds)
{
	char	*cmd_path;
	char	**arr_token;
	pid_t	pid;
	int token_count;

	token_count = token_list_length(cmds->token_list);
	arr_token = malloc(sizeof(char *) * (token_count + 1));
	if (!arr_token)
		if_arr_token_fail(shell);
	if (convert_tokens_to_argv(cmds->token_list, arr_token) == -1)
		if_convert_fail(shell, arr_token);
	if (!arr_token[0])
		return (free(arr_token));
	cmd_path = resolve_command_path(shell, cmds, arr_token);
	if (!cmd_path)
		return ;
	pid = fork();
	if (pid < 0)
		if_fork_fail(cmd_path, arr_token, token_count);
	else if (pid == 0)
		in_child(shell, cmd_path, arr_token);
	else
		store_pids(shell, pid);
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
