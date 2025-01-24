/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 09:39:49 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:23:53 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷   shell -> our structor
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- we iterate over the envp array and print each element
//     2- we print a newline character after each element
/******************************************************************************/

void	print_env(t_shell_data *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		ft_putstr_fd(shell->envp[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
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
//     1- we store the next token in the token list in the token variable
//        because the first token is the env command itself
//     2- we set arg_count to 0 to count the number of arguments provided
//     3- using a while loop, we iterate over the tokens and count the number
//        of arguments
//     4- if the number of arguments is greater than 0, it means we received
//        more than one argument, so we print an error message and return 1
//     5- if the number of arguments us 0
//        we call the print_env function to print all the environment variables
//	 6- we set the exit status to 0 and return 0
/******************************************************************************/
int	ft_env(t_shell_data *shell, t_command *cmd)
{
	int		arg_count;
	t_token	*token;

	token = cmd->token_list->next;
	arg_count = 0;
	while (token)
	{
		arg_count++;
		token = token->next;
	}
	if (arg_count > 0)
	{
		shell->exit_status = 1;
		return (1);
	}
	print_env(shell);
	shell->exit_status = 0;
	return (0);
}
