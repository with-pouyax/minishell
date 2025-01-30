/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:31:39 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:27:35 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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

/*****************************************************************************/
//          No explanation needed.
/*****************************************************************************/

int	handle_invalid_operator(t_shell_data *shell, char *op)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", \
	STDERR_FILENO);
	ft_putstr_fd(op, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	shell->exit_status = 2;
	free(op);
	return (1);
}

/*****************************************************************************/
// 🎯 Purpose  :  process the operator
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//     🏷  i     -> index of the current character in the command string
//     🏷  cmd   -> our command linked list
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using collect_operator() we collect the operator from the command
//        string and store it in op variable.
//        a- if there is no operator we return 1.
//     3- using is_valid_operator() we check if the operator is valid.
//        a- using is_redirection_operator() we check if the operator is a
//           redirection operator.
//           I- if it is a redirection operator we call
//              handle_redirection_operator() and we store the return value
//              of it in ret.
//        b- if it is not a redirection operator we call handle_pipe_operator()
//           and just free the operator and store the return value of it in ret
//
//        c- if ret is 1 it means there is an error and we return 1.
//     4- if the operator is not valid we call handle_invalid_operator() and
//        store the return value of it in ret which is 1.
//     5- if everything is fine we return 0.
//
/******************************************************************************/

int	process_operator(t_shell_data *shell, int *i, t_command *cmd)
{
	char	*op;
	int		ret;

	op = collect_operator(cmd, i);
	if (!op)
		return (1);
	if (is_valid_operator(op))
	{
		if (is_redirection_operator(op))
			ret = handle_redirection_operator(shell, op, cmd, i);
		else
			ret = handle_pipe_operator(op, cmd);
		if (ret)
			return (1);
	}
	else
	{
		ret = handle_invalid_operator(shell, op);
		return (ret);
	}
	return (0);
}
