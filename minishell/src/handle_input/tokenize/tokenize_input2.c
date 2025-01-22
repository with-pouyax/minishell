/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:48:39 by pghajard          #+#    #+#             */
/*   Updated: 2025/01/22 13:48:55 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :  tokenize the command
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//     🏷  cmd   -> our command linked list
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1-  we iterate over the command string using a while loop.
//         a- using skip_cmd_spaces() we skip the whitespaces at the beginning
//            of the command string.
//         b- if there is a character in the command string
//            I- using is_operator_char() we check if the current character is
//               an operator character.
//               - if it is not an operator character we call the
//                 tokenize_command_error() and return its return value, which
//                 is 1.
//         c- if the current character is not an operator character
//            I- using process_word() we process the word and tokenize it.
//               - if there is an error we call tokenize_command_error() and
//                 return its return value, which is 1.
//   2- we will be out of the loop if everything is fine and we return 0.
/******************************************************************************/

int	tokenize_command(t_shell_data *shell, t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->command_string[i])
	{
		skip_cmd_spaces(cmd->command_string, &i);
		if (cmd->command_string[i])
		{
			if (is_operator_char(cmd->command_string[i]))
			{
				if (process_operator(shell, &i, cmd))
					return (tokenize_command_error(cmd));
			}
			else
			{
				if (process_word(shell, cmd->command_string, &i, cmd))
					return (tokenize_command_error(cmd));
			}
		}
	}
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :  skip spaces
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  str -> string we want to skip spaces in.
//     🏷  i   -> index of the current character in the string.
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- using a while loop we iterate over the string characters.
//
/******************************************************************************/

void	skip_cmd_spaces(char *str, int *i)
{
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
}

int	handle_initial_pipe(char *input, int *i)
{
	if (input[*i] == '|' && (input[*i + 1] == '<' || input[*i + 1] == '>') && \
	!ft_isspace(input[*i + 1]))
	{
		(*i)++;
	}
	return (0);
}
/*****************************************************************************/
// 🎯 Purpose  :  collect the operator from the command string and store
//                it in op
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  cmd   -> our command linked list
//     🏷  i     -> index of the current character in the command string
//
// 🔄 Returns   :  op
//
/*****************************************************************************/
// 💡 Notes:
//     1- we call is_operator_char() to check if the current character is an
//        operator character or not.
//        a- if it is an operator character we increment the index.
//        b- if the previous character is < or > and the current character is
//           the same as the previous character we increment the index.
//     2- using ft_substr() we extract the operator from the command string and
//        store it in op.
//        a- if there is an error creating the substring we print an error
//     3- at the end we return op.
//
/******************************************************************************/

char	*collect_operator(t_command *cmd, int *i)
{
	char	*op;
	int		start;

	start = *i;
	(void)start;
	if (is_operator_char(cmd->command_string[*i]))
	{
		(*i)++;
		if ((cmd->command_string[*i - 1] == '<' || cmd->command_string[*i - 1] \
		== '>') && cmd->command_string[*i] == cmd->command_string[*i - 1])
			(*i)++;
	}
	op = ft_substr(cmd->command_string, start, *i - start);
	if (!op)
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
	return (op);
}

/*****************************************************************************/
// 🎯 Purpose  :  handle redirection operator
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  op -> the character we recieved as operator.
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- we store the current operator in cmd->current_op.
//     2- using handle_redirection() we handle the redirection.
//        a- if there is an error we return 1.
//     3- if everything is fine we return 0.
//
/******************************************************************************/

int	handle_redirection_operator(t_shell_data *shell, char *op, \
t_command *cmd, int *i)
{
	cmd->current_op = op;
	if (handle_redirection(shell, cmd->command_string, i, cmd))
		return (1);
	return (0);
}
/*****************************************************************************/
//    here we just free the operator and return 0
/*****************************************************************************/

int	handle_pipe_operator(char *op, t_command *cmd)
{
	(void)cmd;
	free(op);
	return (0);
}