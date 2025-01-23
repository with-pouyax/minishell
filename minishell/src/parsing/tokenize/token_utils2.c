/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 22:40:57 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/22 22:41:04 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  redirections ->
//     🏷  new_redir ->
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- if there is no redirection we put the new redirection into the
//        redirections.
//     2- if there is a redirection we loop over the redirections and put the
//        new redirection at the end.
//     3- we set the redirection number to the current redirection number + 1.
//
/******************************************************************************/

void	add_redirection(t_redirection **redirections, t_redirection *new_redir)
{
	t_redirection	*current;

	if (!*redirections)
		*redirections = new_redir;
	else
	{
		current = *redirections;
		while (current->next)
			current = current->next;
		current->next = new_redir;
		new_redir->redir_number = current->redir_number + 1;
	}
}
/*****************************************************************************/
//                  No explanation needed for this function
/*****************************************************************************/

int	starts_with_operator_char(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  new_redir -> the redirection we want to prepare
//     🏷  filename_or_delimiter
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- we put the delimiter into the redirection structure.
//     2-  we call read_heredoc_content() to read the heredoc content.
//         a- if there is an error we free the delimiter and the redirection
//            structure and return 1.
//     3- at the end we return 0.
//
/******************************************************************************/

int	handle_heredoc_redirection(t_shell_data *shell, t_redirection *new_redir, \
char *filename_or_delimiter)
{
	new_redir->delimiter = filename_or_delimiter;
	if (read_heredoc_content(shell, new_redir))
	{
		free(new_redir->delimiter);
		free(new_redir);
		return (1);
	}
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  word ->
//
// 🔄 Returns   : word
//
/*****************************************************************************/
// 💡 Notes:
//     1- we loop over the word and whenever we find a quote we remove it by
//        shifting the characters to the left.
//     2- at the end we return the word.
//
/******************************************************************************/

char	*rm_quotes(char *word)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (word[i] != '\0')
	{
		if (word[i] != '\'' && word[i] != '\"')
		{
			word[j] = word[i];
			j++;
		}
		i++;
	}
	word[j] = '\0';
	return (word);
}

/*****************************************************************************/
// 🎯 Purpose  :  initialize memory for word and collect the word from the
//                input and store it in word.
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  ctx -> our parse context (shell, input, i, redirection linked list)
//     🏷  word -> the word we collected and expanded
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- we set the word to NULL to avoid any garbage value.
//     2- using collect_word() we collect the word from the input and store it
//        in word.
//        a- if there is an error we return 1.
//     3- if there is no word or the word is empty we free the word
//        and return 1
//     4- if everything is fine we return 0.
/******************************************************************************/

int	init_and_collect_word(t_parse_context *ctx, char **word)
{
	*word = NULL;
	if (collect_word(ctx->input, ctx->i, word, ctx->shell))
		return (1);
	if (!*word || ft_strlen(*word) == 0)
	{
		free(*word);
		return (1);
	}
	return (0);
}