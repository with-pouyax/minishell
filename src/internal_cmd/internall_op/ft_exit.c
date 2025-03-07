/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 09:40:42 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 18:53:09 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷 str
//
// 🔄 Returns   : success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using the initialize_conversion function, we check if the string is
//        empty or contains only spaces and tabs and set the sign accordingly
//        a- if the string is empty or contains only spaces and tabs, we
//        return 0
//     2- using the parse_number function, we convert the string to a long long
//        number and check for overflows and invalid input
//        a- if anything goes wrong, we return 0
//     3- now that we have the number, we multiply it by the sign and store it
//        in the result
//     4- we return 1
//
/******************************************************************************/

int	str_to_long_long(const char *str, long long *result)
{
	int			sign;
	long long	num;

	num = 0;
	*result = 0;
	if (!initialize_conversion(&str, &sign))
		return (0);
	if (!parse_number(str, &num, sign))
		return (0);
	*result = num * sign;
	return (1);
}

void	cleanup_and_exit(t_shell_data *shell, int exit_status)
{
	cleanup(shell);
	rl_clear_history();
	close(shell->saved_stdin);
	close(shell->saved_stdout);
	exit(exit_status);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷   shell -> our structor
//     🏷   cmd -> our command
//     🏷   exit_status -> the exit status
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- we set the exit status to the shell's exit status
//     2- if the token list is not empty
//        a- if the token is numeric
//           I- using the ft_atoi function, we convert the token to integer
//              and mod it by 256, because the exit status should be between 0
//              and 255 and store it in exit_status.
//           II- we move the token to the next token
//           III- if the 3rd token exists
//                A- we print an error message
//                B- we set the exit status to 1
//                C- we return 1
//        b- if the token is not numeric
//           I- we print an error message
//           II- we set the exit status to 2
//     3- we return 0
/******************************************************************************/

int	process_exit_arguments(t_shell_data *shell, t_command *cmd, \
int *exit_status)
{
	t_token	*token;

	token = cmd->token_list->next;
	*exit_status = shell->exit_status;
	if (token)
	{
		if (is_numeric(token->value))
		{
			*exit_status = ft_atoi(token->value) % 256;
			token = token->next;
			if (token)
			{
				print_exit_error("exit: too many arguments\n", \
				&shell->exit_status, 1);
				return (1);
			}
		}
		else
		{
			print_exit_error2("exit: %s: numeric argument required\n",
				token->value, exit_status, 2);
		}
	}
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷   shell -> our structor
//     🏷   cmd -> our command
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using the process_exit_arguments function, we check if the exit
//        command has any arguments and set the exit status accordingly and
//        store its return value in should_not_exit
//     2- if should_not_exit is true, meaning there was an error in the exit
//        command, and we should not exit, we return 1
//     3- if should_not_exit is false, meaning there was no error in the exit
//        command, we call the cleanup_and_exit function to clean up and exit
//     4- we return 0
/******************************************************************************/

int	ft_exit_shell(t_shell_data *shell, t_command *cmd)
{
	int	exit_status;
	int	should_not_exit;

	should_not_exit = process_exit_arguments(shell, cmd, &exit_status);
	if (should_not_exit)
		return (1);
	cleanup_and_exit(shell, exit_status);
	return (0);
}

/*****************************************************************************/
//               No explanation needed
/*****************************************************************************/
int	is_numeric(const char *str)
{
	long long	num;

	return (str_to_long_long(str, &num));
}
