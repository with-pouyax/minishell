#include "../../minishell.h"

/*****************************************************************************/
//       No explanation needed for this function
/*****************************************************************************/

int	initialize_word(char **word)
{
	*word = ft_strdup("");
	if (!*word)
		return (1);
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :  if the current character is a space or an operator
//                character return 1
/*****************************************************************************/

int	should_break(const t_shell_data *shell, char c)
{
	if (!shell->in_single_quote && !shell->in_double_quote)
	{
		if (ft_isspace(c) || is_operator_char(c))
			return (1);
	}
	return (0);
}

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

int	free_word_and_return(char *word, int ret)
{
	free(word);
	return (ret);
}

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


int	add_token_to_command(t_command *cmd, char *word)
{
	if (add_token(word, &cmd->token_list, &(cmd->token_index), 0))
		return (1);
	return (0);
}

int	set_original_value(t_command *cmd, char *original_word)
{
	t_token *last_token;

	last_token = cmd->token_list;
	while (last_token->next)
		last_token = last_token->next;
	last_token->original_value = original_word;
	return (0);
}

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

void	free_expanded_word_arr(char **expanded_word_arr, int j)
{
	while (j > 0)
		free(expanded_word_arr[--j]);
	free(expanded_word_arr);
}

int	handle_token_addition_failure(char **expanded_word_arr, int j, \
char *expanded_word, char *original_word)
{
	free(expanded_word_arr[j]);
	free_expanded_word_arr(expanded_word_arr, j);
	free(expanded_word);
	free(original_word);
	return (1);
}

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

char	**split_expanded_word(char *expanded_word)
{
	char	**expanded_word_arr;

	expanded_word_arr = ft_split(expanded_word, ' ');
	if (!expanded_word_arr)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
	}
	return (expanded_word_arr);
}
void	free_all_resources(char **expanded_word_arr, char *expanded_word, \
char *original_word)
{
	free(expanded_word_arr);
	free(expanded_word);
	free(original_word);
}

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
