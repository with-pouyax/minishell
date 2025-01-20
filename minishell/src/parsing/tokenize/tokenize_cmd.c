/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:30:47 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 22:20:15 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../minishell.h"

void	add_command_to_list(t_shell_data *shell, t_command **last_cmd, \
t_command *cmd)
{
	if (!shell->commands)
		shell->commands = cmd;
	else
		(*last_cmd)->next = cmd;
	*last_cmd = cmd;
}
/*****************************************************************************/
// 🎯 Purpose  : turn command we found into a node and setup its flags
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell   ->  our structure
//     🏷  cmd_str -> is the command we found in the input string separated
//         by pipes
//     🏷  index   -> is the index of the first non-space character in the input
//
// 🔄 Returns   :  cmd linked list
//
/*****************************************************************************/
// 💡 Notes:
//     1- we allocate memory for the cmd node.
//        a- if there is an error we print an error message and return NULL.
//     2- we put the cmd_str we made into the cmd->command_string field.
//     3- we put the index into the cmd->index field.
//     4- we set the cmd->is_recalled to 0.
//     5- we set the is_recalled to 0.
//     6- we set the cmd->token_list to NULL.
//     7- we set the cmd->redirections to NULL.
//     8- we set the cmd->token_index to 0.
//     9- we set the cmd->next to NULL because it is the last node in the
//        linked list.
//     10- we return the cmd.
/******************************************************************************/

t_command	*create_command(t_shell_data *shell, char *cmd_str, int index)
{
	t_command	*cmd;

	(void)shell;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (NULL);
	}
    ft_bzero(cmd, sizeof(t_command));
	cmd->command_string = cmd_str;
	cmd->index = index;
	cmd->is_recalled = 0;
	cmd->token_list = NULL;
	cmd->redirections = NULL;
	cmd->token_index = 0;
	cmd->next = NULL;
	return (cmd);
}

/*****************************************************************************/
// 🎯 Purpose  :  turn input into commands
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  input -> user input
//     🏷  i -> the index of the first non-space character in the input
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- using a while loop we iterate over the input characters as long as
//        the current character is not a pipe.
//        a- if the current character is a single or double quote we skip the
//           quoted word, because we don't want to count the pipes inside the
//           quotes as a pipe.
//        b- if the current character is not a quote we increment the index.
//     2- after our stop, we check if the current character is a pipe and the
//        next character is also a pipe, we print an error message
//        and return -1
//     3- if 1st character after the loop stop is a space, using a while loop
//        we skip the spaces until we find a non-space character.
//     4- if the 1st character after the loop stop is a pipe, we print an error
//        message and return -1.
/******************************************************************************/

int	extract_command_string(char *input, int i)
{
	while (input[i] && input[i] != '|')
	{
		if (input[i] == '\'' || input[i] == '\"')
			i = skip_quotes(input, i);
		else
			i++;
	}
	if (input[i] == '|' && input[i + 1] == '|')
		return (ft_putstr_fd("minishell: syntax error near \
		unexpected token `|'\n", STDERR_FILENO), -1);
	if (input[i] && input[i + 1] == ' ')
	{
		i++;
		while (input[i] && input[i] == ' ')
			i++;
		if (input[i] == '|')
			return (ft_putstr_fd("minishell: syntax error near \
			unexpected token `|'\n", STDERR_FILENO), -1);
	}
	return (i);
}
