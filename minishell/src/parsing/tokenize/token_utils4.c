/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:31:36 by pghajard          #+#    #+#             */
/*   Updated: 2025/01/22 13:35:59 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"


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

/*****************************************************************************/
// 🎯 Purpose  :  process the filename or delimiter in the redirection
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  input -> user input
//     🏷  i -> index of the current character in the input
//     🏷  redir -> our redirection linked list
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     - ctx stands for context and it is a structure that holds the shell
//	   structure, the input, the index of the current character in the input
//	   and the redirection linked list. we use it to pass less parameters to
//	   the functions.
//     1- using collect_and_expand_redirection_word() we collect and expand the
//        redirection word, redirection word is the filename or the delimiter.
//        a- if there is no redirection word or the redirection word is empty
//           we free words.expanded and words.original and return 1.
//     2- using validate_expanded_word() we validate the expanded word, it will
//        be invalid if it is an operator or starts with an operator.
//        a- if it is invalid we free words.expanded and words.original and
//           return 1.
//     3- using assign_redirection() we assign the redirection word to the
//        redirection linked list.
//     4- now that everything is fine we return 0.
//
/******************************************************************************/

int	process_filename_or_delimiter(t_shell_data *shell, char *input, \
int *i, t_redirection *redir)
{
	t_expanded_words	words;
	t_parse_context		ctx;

	ctx.shell = shell;
	ctx.input = input;
	ctx.i = i;
	ctx.redir = redir;
	if (collect_and_expand_redirection_word(shell, &ctx, &words))
		return (handle_missing_filename_error(shell, redir));
	if (!words.expanded || ft_strlen(words.expanded) == 0)
	{
		free(words.expanded);
		free(words.original);
		return (handle_missing_filename_error(shell, redir));
	}
	if (validate_expanded_word(shell, words.expanded))
	{
		free(words.expanded);
		free(words.original);
		free_redirections(redir);
		return (1);
	}
	assign_redirection(shell, words.expanded, words.original);
	return (0);
}

int	handle_unexpected_token_error(t_shell_data *shell, t_redirection *new_redir, \
char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", \
	STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	free(new_redir);
	shell->exit_status = 2;
	return (1);
}
