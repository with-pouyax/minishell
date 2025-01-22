/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:31:36 by pghajard          #+#    #+#             */
/*   Updated: 2025/01/22 13:36:35 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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

/*****************************************************************************/
// 🎯 Purpose  :  collec and expand redirection word
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  words -> our expanded words structure
//     🏷  word  -> word is the filename or delimiter
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using save_and_expand_word() we save and expand the word and store it
//        in words.expanded and words.original.
//        a- if there is an error we free the word and return 1.
//     2- at the end we return 0.
//
/******************************************************************************/

int	handle_non_heredoc(t_shell_data *shell, char *word, t_expanded_words *words)
{
	if (save_and_expand_word(shell, word, &(words->expanded), \
&(words->original)))
	{
		free(word);
		return (1);
	}
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  word  -> word is the filename or delimiter
//     🏷  words -> our expanded words structure
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using rm_quotes() we remove the quotes from the word.
//     2- we store the word in words.expanded and words.original.
//     3- if there is an error duplicating the word in words.expanded or
//        words.original we free the word
//        a- if there is a word in words.expanded we free it.
//        b- if there is a word in words.original we free it.
//        c- we set the error flag to 4.
//        d- we free the word and return 1.
//     4- at the end we return 0.
//
/******************************************************************************/

int	process_heredoc_word(t_shell_data *shell, char *word, \
t_expanded_words *words)
{
	rm_quotes(word);
	words->expanded = ft_strdup(word);
	words->original = ft_strdup(word);
	if (!words->expanded || !words->original)
	{
		if (words->expanded)
			free(words->expanded);
		if (words->original)
			free(words->original);
		shell->error_flag = 4;
		free(word);
		return (1);
	}
	return (0);
}