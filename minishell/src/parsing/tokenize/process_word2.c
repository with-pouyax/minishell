/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_word2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 22:24:55 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/22 22:26:41 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :  collec and expand redirection word
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷   input -> user input
//     🏷   i -> index of the current character in the input
//     🏷   word -> a pointer to a string that we store the word in
//     🏷   shell -> our structure
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the word is not initialized
//        a- we initialize it using initialize_word()
//           I- if there is an error we set the error flag to 4 and return 1.
//     2- using a while loop we iterate over the input characters
//        a- we check if we should break using should_break(), we should break
//           if the current character is a space or an operator character.
//           I- if we should break we break the loop.
//        b- using process_character() we process the current character and
//           store the return value in ret.
//           I- if there is an error we return 1.
//     3- now that we are out of the loop we check if there are unclosed quotes
//        using handle_unclosed_quotes().
//        a- if there are unclosed quotes we return 1.
//     4- using finalize_word() we finalize the word, by finalizing I mean we
//        check if the word is empty and free it if it is.
/******************************************************************************/

int	collect_word(char *input, int *i, char **word, \
t_shell_data *shell)
{
	int	ret;

	if (!*word)
	{
		if (initialize_word(word))
		{
			shell->error_flag = 4;
			return (1);
		}
	}
	while (input[*i])
	{
		if (should_break(shell, input[*i]))
			break ;
		ret = process_character(shell, input[*i], word);
		if (ret)
			return (1);
		(*i)++;
	}
	if (handle_unclosed_quotes(shell, word))
		return (1);
	return (finalize_word(word));
}
/*****************************************************************************/
//           No explanation needed for this function
/*****************************************************************************/

int	free_word_and_return(char *word, int ret)
{
	free(word);
	return (ret);
}

/*****************************************************************************/
//          No explanation needed for this function
/*****************************************************************************/


int	free_original_and_expanded_and_return(char *original_word, \
char *expanded_word, int ret)
{
	free(original_word);
	if (expanded_word)
		free(expanded_word);
	return (ret);
}

/*****************************************************************************/
// 🎯 Purpose  :  collec and expand redirection word
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  word  -> word is the filename or delimiter
//     🏷  expanded_word is the word after expansion
//     🏷  original_word is the word before expansion
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using strdup() we duplicate the word and store it in original_word.
//        a- if there is an error we print an error message and return 1.
//     2- using expand_variables_in_token() we expand the variables in the word
//        and store the result in expanded_word.
//        a- if there is an error we free the original_word and return 1.
//     3- at the end we return 0.
//
/******************************************************************************/

int	save_and_expand_word(t_shell_data *shell, char *word, \
char **expanded_word, char **original_word)
{
	*original_word = ft_strdup(word);
	if (!*original_word)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (1);
	}
	*expanded_word = expand_variables_in_token(shell, word);
	if (!*expanded_word)
	{
		free(*original_word);
		return (1);
	}
	return (0);
}

/*****************************************************************************/
//              No explanation needed for this function
/*****************************************************************************/

int	add_token_to_command(t_command *cmd, char *word)
{
	if (add_token(word, &cmd->token_list, &(cmd->token_index), 0))
		return (1);
	return (0);
}

