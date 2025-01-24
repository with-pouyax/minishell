/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 22:42:19 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:27:09 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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

/*****************************************************************************/
// 🎯 Purpose  :  collec and expand redirection word
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  ctx -> our parse context (shell, input, i, redirection linked list)
//     🏷  words -> our expanded words structure
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using init_and_collect_word() we collect and expand the redirection
//        word and store it in word.
//        a- if there is an error we return 1.
//     2- if the redirection type is not heredoc we call handle_non_heredoc()
//        we call handle_non_heredoc() and store the return value of it
//        in error
//        a- if there is an error we return 1.
//     3- if the redirection type is heredoc we call process_heredoc_word()
//        and store the return value of it in error.
//        a- if there is an error we return 1.
//     4- at the end we free the word and return 0.
//
/******************************************************************************/

int	collect_and_expand_redirection_word(t_shell_data *shell, \
		t_parse_context *ctx, t_expanded_words *words)
{
	char	*word;
	int		error;

	error = init_and_collect_word(ctx, &word);
	if (error)
		return (1);
	if (ctx->redir->type != REDIR_HEREDOC)
	{
		error = handle_non_heredoc(shell, word, words);
		if (error)
			return (1);
	}
	else
	{
		error = process_heredoc_word(shell, word, words);
		if (error)
			return (1);
	}
	free(word);
	return (0);
}

/*****************************************************************************/
//      here we assign the redirection word to the redirection linked list
/*****************************************************************************/

void	assign_redirection(t_shell_data *shell, char *expanded_word, \
char *original_word)
{
	shell->filename_or_delimiter = expanded_word;
	free(original_word);
}

/*****************************************************************************/
// 🎯 Purpose  :  process the filename or delimiter in the redirection
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  expanded_word
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using is_valid_operator() we check if the expanded word is a valid
//        operator.
//        a- if it is return 1 it means it is an invalid operator, so if it is
//           an invalid operator or starts with an operator.
//           I- we print an error message.
//           II- we set the exit status to 2.
//           III- we return 1.
//     2- if it is a valid operator and does not start with an operator we
//        return 0.
//
//
/******************************************************************************/

int validate_expanded_word(t_shell_data *shell, char *expanded_word)
{
	if (is_valid_operator(expanded_word) || \
	starts_with_operator_char(expanded_word[0]))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", \
		STDERR_FILENO);
		ft_putstr_fd(expanded_word, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}
