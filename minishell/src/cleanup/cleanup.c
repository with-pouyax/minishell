/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:34:46 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 00:40:33 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	free_and_return(char *str)
{
	free(str);
	return (1);
}

/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- if there is no envp we return immediately.
//     2- using a while loop we are looping through our envp array and free
//        each element.
//     3- after we free all the elements we free the envp array and set it to
//        NULL.
/******************************************************************************/

void	free_envp(t_shell_data *shell)
{
	int	i;

	i = 0;
	if (!shell->envp)
		return ;
	while (shell->envp[i])
	{
		free(shell->envp[i]);
		i++;
	}
	free(shell->envp);
	shell->envp = NULL;
}

/*****************************************************************************/
// 🎯 Purpose  :    cleaning up all resources
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- we free the input and set it to NULL.
//     2- we free the full_input and set it to NULL.
//     3- we call free_commands() to free all the commands.
//     4- we call free_envp() to free envp array.
//     5- if there is a prev_dir we free it and set it to NULL.
//	   	  in prev_dir we store the previous directory, we used it in cd command
//        to go back to the previous directory.
/******************************************************************************/

void	cleanup(t_shell_data *shell)
{
	free(shell->input);
	shell->input = NULL;
	free(shell->full_input);
	shell->full_input = NULL;
	free_commands(shell);
	free_envp(shell);
	if (shell->prev_dir)
	{
		free(shell->prev_dir);
		shell->prev_dir = NULL;
	}
}

int	tokenize_command_error(t_command *cmd)
{
	if (cmd->token_list)
	{
		free_tokens(cmd->token_list);
		cmd->token_list = NULL;
	}
	if (cmd->redirections)
	{
		free_redirections(cmd->redirections);
		cmd->redirections = NULL;
	}
	if (cmd->current_op)
	{
		free(cmd->current_op);
		cmd->current_op = NULL;
	}
	return (1);
}
