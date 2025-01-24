/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:36:02 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:20:31 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :    free all the tokens
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  token_list -> linked list of tokens
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- we loop through the linked list of tokens.
//        a- we put the next token in next_token so we can free the current
//           token and don't lose the access to the next token.
//        b- we free the value of the token.
//        c- if the token has an original_value we free it.
//        d- if the token is a heredoc token using free_heredoc_token() we free
//           the heredoc token.
//        e- we free the token.
/******************************************************************************/

void	free_tokens(t_token *token_list)
{
	t_token	*token;
	t_token	*next_token;

	token = token_list;
	while (token)
	{
		next_token = token->next;
		free(token->value);
		if (token->original_value)
			free(token->original_value);
		if (token->is_heredoc)
			free_heredoc_token(token);
		free(token);
		token = next_token;
	}
}

/*****************************************************************************/
// 🎯 Purpose  :    free here document token
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  token -> here document token
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the token contains a heredoc file we unlink it and free it using
//        unlink(). unlink() deletes a name from the filesystem and if that
//        name was the last link to a file and no processes have the file open
//        the file is deleted and the space it was using is made available for
//        reuse.
//     2- if the token has a heredoc_delimiter we free it.
/******************************************************************************/

void	free_heredoc_token(t_token *token)
{
	if (token->heredoc_file)
	{
		unlink(token->heredoc_file);
		free(token->heredoc_file);
	}
	if (token->heredoc_delimiter)
		free(token->heredoc_delimiter);
}

/*****************************************************************************/
// 🎯 Purpose  :    free all resources
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- using free_commands() we free all the commands.
//     2- we free the input and full_input.
//     3- we set the input and full_input to NULL.
/******************************************************************************/

void	free_shell_resources(t_shell_data *shell)
{
	free_commands(shell);
	free(shell->input);
	free(shell->full_input);
	shell->input = NULL;
	shell->full_input = NULL;
}
/*****************************************************************************/
//                            🚀 free_commands 🚀
/*****************************************************************************/
// 🎯 Purpose  :    free all the commands
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- using a while loop we are looping through our commands linked list
//        nodes one by one.
//        a- using free_tokens() we free the token_list.
//        b- using free_redirections() we free the redirections.
//        c- we free the command_string.
//        d- we free the command node.
//     2- now that we have freed all the commands nodes we set the
//        shell->commands to NULL.
/******************************************************************************/

void	free_commands(t_shell_data *shell)
{
	t_command	*cmd;
	t_command	*next_cmd;

	cmd = shell->commands;
	while (cmd)
	{
		next_cmd = cmd->next;
		free_tokens(cmd->token_list);
		free_redirections(cmd->redirections);
		free(cmd->command_string);
		free(cmd);
		cmd = next_cmd;
	}
	shell->commands = NULL;
}
