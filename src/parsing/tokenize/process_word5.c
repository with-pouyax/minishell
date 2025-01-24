/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_word5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 22:33:45 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:26:44 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  cmd   -> our command linked list
//     🏷  original_word -> the original word
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- using a while loop we iterate backwards over the command and free
//        each token.
//     2- we free the original word.
//     3- we return 0.
//
/******************************************************************************/
int	set_original_value(t_command *cmd, char *original_word)
{
	t_token	*last_token;

	last_token = cmd->token_list;
	while (last_token->next)
		last_token = last_token->next;
	last_token->original_value = original_word;
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//     🏷  i     -> index of the current character in the command string
//     🏷  cmd   -> our command linked list
//     🏷  expanded_word ->
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- first allocate memory for the word and if failed we print an error
//        message and return 1.
//     2- using save_and_expand_word() we save and expand the word.
//        a- if there is an error we call free_word_and_return() and return
//           its return value.
//     3- we free the word.
//     4- we return 0.
//
/******************************************************************************/

int	collect_and_expand_word(t_shell_data *shell, char *input, \
int *i, char **expanded_word)
{
	char	*word;

	word = ft_strdup("");
	if (!word || collect_word(input, i, &word, shell))
	{
		ft_putstr_fd("minishell: memory allocation error\n", \
		STDERR_FILENO);
		return (free_word_and_return(word, 1));
	}
	if (save_and_expand_word(shell, word, expanded_word, \
	&shell->original_word))
		return (free_word_and_return(word, 1));
	free(word);
	return (0);
}
/*****************************************************************************/
//       using a while loop we iterate backwards over the command and free
//       each token.
/*****************************************************************************/
void	free_expanded_word_arr(char **expanded_word_arr, int j)
{
	while (j > 0)
		free(expanded_word_arr[--j]);
	free(expanded_word_arr);
}

/*****************************************************************************/
//             free all resources and return 1
/*****************************************************************************/

int	handle_token_addition_failure(char **expanded_word_arr, int j, \
char *expanded_word, char *original_word)
{
	free(expanded_word_arr[j]);
	free_expanded_word_arr(expanded_word_arr, j);
	free(expanded_word);
	free(original_word);
	return (1);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  cmd   -> our command linked list
//     🏷  expanded_word_arr -> the expanded word array
//
// 🔄 Returns   :  expanded word array
//
/*****************************************************************************/
// 💡 Notes:
//     1- using a while loop we iterate over the expanded word array.
//        a- using add_token_to_command() we add the token to the command.
//           I- if there is an error we call handle_token_addition_failure()
//              and return its return value.
//     2- at the end we return 0.
//
//
/******************************************************************************/

int	add_tokens_to_command(t_command *cmd, char **expanded_word_arr)
{
	int	j;

	j = 0;
	while (expanded_word_arr[j])
	{
		if (add_token_to_command(cmd, expanded_word_arr[j]))
			return (handle_token_addition_failure(expanded_word_arr, j, \
			expanded_word_arr[j], NULL));
		j++;
	}
	return (0);
}