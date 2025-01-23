/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:49:49 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:30:51 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	calc_pipe_nb(t_shell_data *shell)
{
	char	*full_input;
	int		pipe_nb;

	full_input = shell->full_input;
	pipe_nb = 0;
	while (*full_input)
	{
		if (*full_input == '|')
			pipe_nb++;
		full_input++;
	}
	return (pipe_nb);
}

int	calc_cmds_nb(t_shell_data *shell)
{
	char	*input;
	int		cmd_count;
	int		i;

	input = shell->full_input;
	cmd_count = 0;
	i = 0;
	if (!input || !*input)
		return (0);
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (input[i] && input[i] != '|')
		{
			cmd_count++;
			while (input[i] && input[i] != '|')
				i++;
		}
		if (input[i] == '|')
			i++;
	}
	return (cmd_count);
}

/*****************************************************************************/
//                              🚀 init_shell 🚀
/*****************************************************************************/
// 🎯 Purpose  :    initialize the shell struct with default values
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell  -> pointer to the shell struct
//
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- fill it later
/******************************************************************************/

void	init_shell(t_shell_data *shell)
{
	shell->input = NULL;
	shell->full_input = NULL;
	shell->exit_status = 0;
	shell->commands = NULL;
	shell->error_flag = 0;
	shell->signal_status = 0;
	shell->in_single_quote = 0;
	shell->in_double_quote = 0;
	shell->prev_char = '\0';
	shell->var_not_found_flag = 0;
	shell->prev_dir = getcwd(NULL, 0);
	shell->in_heredoc = 0;
	shell->expanded = 0;
	shell->double_quoted = 0;
	shell->original_word = NULL;
	if (!shell->prev_dir)
	{
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
}
