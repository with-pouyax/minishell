/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_tokenize.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:04:15 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 10:05:22 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	count_commands_and_pipes(t_shell_data *shell)
{
	int			cmd_count;
	t_command	*cmd;

	cmd_count = 0;
	cmd = shell->commands;
	while (cmd)
	{
		cmd_count++;
		cmd = cmd->next;
	}
	shell->cmds_nb = cmd_count;
	shell->pipe_nb = cmd_count - 1;
}

void	handle_tokenization_failure(t_shell_data *shell)
{
	free_commands(shell);
	shell->commands = NULL;
	handle_tokenization_error(shell, shell->error_flag);
}
/*****************************************************************************/
// 🎯 Purpose  :  turn input into commands
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- if there is no input we return 1.
//     2- shell->input[i] is the current character in the input.
//     3- using a while loop we iterate over the input characters.
//        a- using skip_spaces() we skip the whitespaces.
//        b- using process_input_segment() we process the input segment.
//        c- by process I mean we tokenize the input segment into commands.
//           I- if there is an error in the process_input_segment() we return 1
//     4- after we have processed the input we check if there is an error flag.
//        a- if there is an error flag we call handle_tokenization_failure() to
//           handle the error.
//        b- if there is no error flag we call count_commands_and_pipes() to
//           count the number of commands and pipes.
//     5- if everything is fine we return 0.
/******************************************************************************/

int	split_cmd_tokenize(t_shell_data *shell)
{
	int				i;
	int				cmd_index;
	t_command		*last_cmd;

	i = 0;
	cmd_index = 0;
	last_cmd = NULL;
	shell->error_flag = 0;
	if (!shell->input)
		return (1);
	while (shell->input[i])
	{
		skip_spaces(shell, &i);
		if (process_input_segment(shell, &i, &cmd_index, &last_cmd) > 0)
			return (1);
	}
	if (shell->error_flag)
		handle_tokenization_failure(shell);
	else
		count_commands_and_pipes(shell);
	return (0);
}
