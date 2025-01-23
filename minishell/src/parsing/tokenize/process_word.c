/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:02:05 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/22 22:33:42 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"



/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  expanded_word -> the expanded word
//
// 🔄 Returns   :  expanded word array
//
/*****************************************************************************/
// 💡 Notes:
//     1- using ft_split() we split the expanded word and store it in
//        expanded_word_arr.
//     2- if there is an error we print an error message.
//     3- we return expanded_word_arr.
//
//
/******************************************************************************/

char	**split_expanded_word(char *expanded_word)
{
	char	**expanded_word_arr;

    (void)expanded_word;
	expanded_word_arr = ft_split(expanded_word, ' ');
	if (!expanded_word_arr)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
	}
	return (expanded_word_arr);
}
/*****************************************************************************/
//               No explanation needed for this function
/******************************************************************************/

void	free_all_resources(char **expanded_word_arr, char *expanded_word, \
char *original_word)
{
	free(expanded_word_arr);
	free(expanded_word);
	free(original_word);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//     🏷  cmd   -> our command linked list
//     🏷  expanded_word -> the expanded word
//     🏷  original_word -> the original word
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- using split_expanded_word() we split the expanded word and store it
//        in expanded_word_arr, we need to split the expanded word because we
//        need to tokenize each word separately.
//     2- if there is an error we free the expanded word and original word and
//        return 1.
//     3- using add_tokens_to_command() we tokenize each word and add it to the
//        command linked list.
//        a- if there is an error it will return 1 and exit the function.
//        b- if there is no error we free all the resources of command and
//           return 0.
//
//
/******************************************************************************/

int	handle_expanded(t_shell_data *shell, t_command *cmd, \
char *expanded_word, char *original_word)
{
	char	**expanded_word_arr;

	expanded_word_arr = split_expanded_word(expanded_word);
	if (!expanded_word_arr)
	{
		free(expanded_word);
		free(original_word);
		return (1);
	}
	if (add_tokens_to_command(cmd, expanded_word_arr))
		return (1);
	free_all_resources(expanded_word_arr, expanded_word, original_word);
	shell->expanded = 0;
	shell->double_quoted = 0;
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//     🏷  cmd   -> our command linked list
//     🏷  expanded_word -> the expanded word
//     🏷  original_word -> the original word
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- using add_token_to_command() we add the expanded word to the command.
//        a- if there is we return free_original_and_expanded_and_return() and
//           return its return value.
//     2- using set_original_value() we set the original value of the command.
//        a- if there is an error we return 1.
//     3- at the end we return 0.
//
/******************************************************************************/

int	handle_non_expanded(t_shell_data *shell, t_command *cmd, \
char *expanded_word, char *original_word)
{
	(void)shell;
	if (add_token_to_command(cmd, expanded_word))
		return (free_original_and_expanded_and_return(original_word, \
		expanded_word, 1));
	if (set_original_value(cmd, original_word))
		return (1);
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//     🏷  cmd   -> our command linked list
//     🏷  input -> user input
//     🏷  i     -> index of the current character in the command string
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- using collect_and_expand_word() we collect and expand the word.
//        a- if there is an error we return 1.
//     2- we set the original word to the shell structure.
//     3- if the shell->expanded exists and we are not in a double quote
//        a- using handle_expanded() we handle the expanded word, meaning we
//           split the expanded word and add the tokens to the command.
//           I- if there is an error we return 1.
//     4- if the shell->expanded does not exist or we are in a double quote
//        a- using handle_non_expanded() we handle the non expanded word, meaning
//           we add the word to the command.
//           I- if there is an error we return 1.
//     5- at the end we return 0.
//
/******************************************************************************/

int	process_word(t_shell_data *shell, char *input, int *i, t_command *cmd)
{
	char	*expanded_word;
	char	*original_word;


	if (collect_and_expand_word(shell, input, i, &expanded_word))
		return (1);
	original_word = shell->original_word;
	if (shell->expanded && !shell->double_quoted)
	{
		if (handle_expanded(shell, cmd, expanded_word, original_word))
			return (1);
	}
	else
	{
		if (handle_non_expanded(shell, cmd, expanded_word, original_word))
			return (1);
	}
	return (0);
}
