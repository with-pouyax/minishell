/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_segment.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 09:57:38 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/21 15:45:11 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :  remove the spaces at the end of the cmd_str
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  cmd_str -> cmd_str is the command string that we
//         have split from the whole input string
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- if there is no cmd_str we return immediately.
//     2- we find the length of the cmd_str.
//     3- using a while loop we loop backwards over the cmd_str and  change
//        each space character to '\0'.
/******************************************************************************/

void	trim_trailing_spaces(char *cmd_str)
{
	int	len;

	if (!cmd_str)
		return ;
	len = ft_strlen(cmd_str);
	while (len > 0 && ft_isspace(cmd_str[len - 1]))
		cmd_str[--len] = '\0';
}

/*****************************************************************************/
//          No explanation needed for this function
/******************************************************************************/

int	handle_command_creation_error(t_shell_data *shell,
				char *cmd_str, t_command *cmd)
{
	free(cmd_str);
	free(cmd);
	shell->error_flag = 3;
	return (1);
}

/*****************************************************************************/
// 🎯 Purpose  :  turn input into commands
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//     🏷  i -> the index of the first non-space character in the input
//     🏷  cmd_index -> the index of the current command
//     🏷  last_cmd -> the last command in the linked list
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- using extract_command_string() we extract the command string from the
//        input separated by pipes.
//        a- if there is an error we set the error flag to 1 and return 1.
//     2- using ft_substr() we extract the command we have found and store it
//        in cmd_str.
//        a- if there is an error we print an error message and set the error
//           flag to 2 and return 1.
//     3- using trim_trailing_spaces() we remove the spaces at the end of the
//        comd_str.
//     4- using create_command() we create a new command node and setup its
//        flags and return it.
//     5- using tokenize_command() we tokenize the cmd_str and store the tokens
//        in cmd token_list.
//        a- if there is no cmd or there is an error in the tokenize_command()
//           we call handle_command_creation_error() to handle the error and
//           return the return value of it which is 1.
//     6- using add_command_to_list() we add the cmd to the linked list of
//        commands and update the last_cmd.
//     7- if the current character is a pipe we increment the index because we
//        have already processed the pipe character in the
//        extract_command_string() and we don't want to process it again.
//     8- if everything is fine we return 0.
/******************************************************************************/

int	process_input_segment(t_shell_data *shell, int *i, int *cmd_index, \
t_command **last_cmd)
{
	int					start;
	char				*cmd_str;
	t_command			*cmd;

	start = *i;
	(void)start;
	*i = extract_command_string(shell->input, *i);
	if (*i == -1)
		return (shell->error_flag = 1, 1);
	cmd_str = ft_substr(shell->input, start, *i - start);
	if (!cmd_str)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (shell->error_flag = 2, 1);
	}
	trim_trailing_spaces(cmd_str);
	cmd = create_command(shell, cmd_str, (*cmd_index)++);
	if (!cmd || tokenize_command(shell, cmd))
		return (handle_command_creation_error(shell, cmd_str, cmd));
	add_command_to_list(shell, last_cmd, cmd);
	if (shell->input[*i] == '|')
		(*i)++;
	return (0);
}
