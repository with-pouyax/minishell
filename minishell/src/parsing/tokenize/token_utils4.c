/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 22:43:13 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/22 22:43:20 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"



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

/*****************************************************************************/
// 🎯 Purpose  :  process the filename or delimiter in the redirection
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  new_redir  ->
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//
//     1- if the shell error flag is 4 we print an Error
//     2- if the shell error flag is not 4 we print a syntax error
//     3- we free the new redirection
//     4- we set the exit status to 2
//     5- we return 1
//
/******************************************************************************/

int	handle_missing_filename_error(t_shell_data *shell, \
t_redirection *new_redir)
{
	if (shell->error_flag == 4)
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
	else
		ft_putstr_fd("syntax error near unexpected token `newline'\n", \
		STDERR_FILENO);
	free(new_redir);
	shell->exit_status = 2;
	return (1);
}
/*****************************************************************************/
// 💡 Notes:
//     1- no explanation needed.
//
/******************************************************************************/
void	ft_putstr_fd2(const char *s, int fd)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

/*****************************************************************************/
// 💡 Notes:
//     1- no explanation needed.
//
/******************************************************************************/

int	handle_syntax_error_s(t_shell_data *shell, t_redirection *new_redir, \
const char *unexpected_token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", \
	STDERR_FILENO);
	ft_putstr_fd2(unexpected_token, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	free(new_redir);
	shell->exit_status = 2;
	return (1);
}