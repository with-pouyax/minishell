/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:18:06 by pghajard          #+#    #+#             */
/*   Updated: 2025/01/22 13:18:14 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷   shell -> our structure
//     🏷   c -> the current character
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the current character is a single quote character and we are not
//        in a double quote we toggle the single quote.
//     2- if the current character is a double quote character and we are not
//        in a single quote we toggle the double quote.
/******************************************************************************/

void	toggle_quotes(t_shell_data *shell, char c)
{
	if (c == '\'' && !shell->in_double_quote)
		shell->in_single_quote = !shell->in_single_quote;
	else if (c == '\"' && !shell->in_single_quote)
		shell->in_double_quote = !shell->in_double_quote;
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷   word -> a pointer to a string that we store the word in
//     🏷   shell -> our structure
//     🏷   c -> the current character
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using add_char_to_token() we add the current character to the word.
//        a- if there is an error we free the word and return 1.
//     2- we set the previous character to the current character in the shell
//        structure.
//     3- we return 0.
/******************************************************************************/

int	add_current_char(t_shell_data *shell, char c, char **word)
{
	int	ret;

	ret = add_char_to_token(word, c);
	if (ret)
	{
		free(*word);
		return (1);
	}
	shell->prev_char = c;
	return (0);
}

/*****************************************************************************/
//         No explanation needed for this function
/*****************************************************************************/

int	handle_unclosed_quotes(t_shell_data *shell, char **word)
{
	if (shell->in_single_quote || shell->in_double_quote)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", \
		STDERR_FILENO);
		free(*word);
		return (1);
	}
	return (0);
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷   word -> a pointer to a string that we store the word in
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the length of the word is 0 we free the word and set it to NULL.
//     2- we return 0.
/******************************************************************************/

int	finalize_word(char **word)
{
	if (ft_strlen(*word) == 0)
	{
		free(*word);
		*word = NULL;
	}
	return (0);
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷   word -> a pointer to a string that we store the word in
//     🏷   shell -> our structure
//     🏷   c -> the current character
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the current character is a quote character
//        a- using toggle_quotes() we change the quote flag, we need to change
//           the quote flag because we need to know if we are in a quote or not
//     2- using add_current_char() we add the current character to the word.
//        a- if there is an error we return 1.
//	   3- if there is no error we return 0.
/******************************************************************************/

int	process_character(t_shell_data *shell, char c, char **word)
{
	if ((c == '\'' && !shell->in_double_quote) || (c == '\"' && \
	!shell->in_single_quote))
	{
		toggle_quotes(shell, c);
	}
	if (add_current_char(shell, c, word))
		return (1);
	return (0);
}
