#include "../../minishell.h"

int	initialize_word(char **word)
{
    *word = ft_strdup("");
    if (!*word)
        return (1);
    return (0);
}

// Determine if the loop should break
int	should_break(const t_shell_data *shell, char c)
{
    if (!shell->in_single_quote && !shell->in_double_quote)                      //if we are not in a single or double quote
    {
        if (ft_isspace(c) || is_operator_char(c))                                //if the character is a space or an operator
            return (1);                                                          // return 1
    }
    return (0);
}

// Toggle the quote flags based on the current character
void	toggle_quotes(t_shell_data *shell, char c)
{
    if (c == '\'' && !shell->in_double_quote)
        shell->in_single_quote = !shell->in_single_quote;
    else if (c == '\"' && !shell->in_single_quote)
        shell->in_double_quote = !shell->in_double_quote;
}

// Add the current character to the word and update prev_char
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

// Handle unclosed quotes and report syntax errors
int	handle_unclosed_quotes(t_shell_data *shell, char **word)
{
    if (shell->in_single_quote || shell->in_double_quote)
    {
        ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR_FILENO);
        free(*word);
        return (1);
    }
    return (0);
}

// Finalize the word by setting it to NULL if it's empty
int	finalize_word(char **word)
{
    if (ft_strlen(*word) == 0)
    {
        free(*word);
        *word = NULL;
    }
    return (0);
}

// Process a single character, toggling quotes and adding to word
int	process_character(t_shell_data *shell, char c, char **word)
{
    if ((c == '\'' && !shell->in_double_quote) || (c == '\"' && !shell->in_single_quote))     //if the character is a single not inside a double quote or a double quote not inside a single quote
    {
        toggle_quotes(shell, c);                                                              //toggle the quotes, i.e. if we are in a single quote, we exit it and vice versa
    }
    if (add_current_char(shell, c, word))                                                     //add the current character to the word
        return (1);
    return (0);
}

// Refactored collect_word function
int	collect_word(char *input, int *i, char **word, t_shell_data *shell)
{
    int	ret;

    if (!*word)                                                           // If the word is NULL, initialize it
    {
        if (initialize_word(word))                                        // allocate memory for the word
            return (1);
    }
    while (input[*i])                                                     // loop through the input string
    {
        if (should_break(shell, input[*i]))                               //if we are not in a single or double quote and the character is a space or an operator
            break;                                                        // break the loop
        ret = process_character(shell, input[*i], word);                  // process the character and store it in the word
        if (ret)
            return (1);
        (*i)++;
    }
    if (handle_unclosed_quotes(shell, word))                               // we check the word for unclosed quotes and report syntax errors if any are found
        return (1);
    return (finalize_word(word));                                          // we check if the word is empty or not
}



//======================================================

// Frees the word and returns the specified value
int	free_word_and_return(char *word, int ret)
{
    free(word);
    return (ret);
}

// Frees the original_word and expanded_word (if not NULL) and returns the specified value
int	free_original_and_expanded_and_return(char *original_word, char *expanded_word, int ret)
{
    free(original_word);
    if (expanded_word)
        free(expanded_word);
    return (ret);
}

// Saves the original word and expands it
int	save_and_expand_word(t_shell_data *shell, char *word, char **expanded_word, char **original_word)
{
    *original_word = ft_strdup(word);                                      //first we allocate memory for the original word and store the word in it
    if (!*original_word)
        return (1);
    *expanded_word = expand_variables_in_token(shell, word);               //we expand the word
    if (!*expanded_word)
    {
        free(*original_word);
        return (1);
    }
    return (0);
}

// Adds the token to the command's token list
int	add_token_to_command(t_command *cmd, char *word)
{
    if (add_token(word, &cmd->token_list, &(cmd->token_index), 0))
        return (1);
    return (0);
}

// Sets the original_value field in the last token
int	set_original_value(t_command *cmd, char *original_word)
{
    t_token *last_token;

    last_token = cmd->token_list;
    while (last_token->next)
        last_token = last_token->next;
    last_token->original_value = original_word;
    return (0);
}

static int	collect_and_expand_word(t_shell_data *shell, char *input, int *i, char **expanded_word)
{
	char	*word;

	word = ft_strdup("");
	if (!word || collect_word(input, i, &word, shell))
    {
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (free_word_and_return(word, 1));
    }
	if (save_and_expand_word(shell, word, expanded_word, &shell->original_word))
		return (free_word_and_return(word, 1));
	free(word);
	return (0);
}

static void	free_expanded_word_arr(char **expanded_word_arr, int j)
{
	while (j > 0)
		free(expanded_word_arr[--j]);
	free(expanded_word_arr);
}

static int	handle_token_addition_failure(char **expanded_word_arr, int j, char *expanded_word, char *original_word)
{
	free(expanded_word_arr[j]);
	free_expanded_word_arr(expanded_word_arr, j);
	free(expanded_word);
	free(original_word);
	return (1);
}

static int	add_tokens_to_command(t_command *cmd, char **expanded_word_arr)
{
	int	j;

	j = 0;
	while (expanded_word_arr[j])
	{
		if (add_token_to_command(cmd, expanded_word_arr[j]))
			return (handle_token_addition_failure(expanded_word_arr, j, expanded_word_arr[j], NULL));
		j++;
	}
	return (0);
}

static char	**split_expanded_word(char *expanded_word)
{
	char	**expanded_word_arr;

	expanded_word_arr = ft_split(expanded_word, ' ');
	return (expanded_word_arr);
}
static void	free_all_resources(char **expanded_word_arr, char *expanded_word, char *original_word)
{
	free(expanded_word_arr);
	free(expanded_word);
	free(original_word);
}

static int	handle_expanded(t_shell_data *shell, t_command *cmd, char *expanded_word, char *original_word)
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

static int	handle_non_expanded(t_shell_data *shell, t_command *cmd, char *expanded_word, char *original_word)
{
    (void)shell;
	if (add_token_to_command(cmd, expanded_word))
		return (free_original_and_expanded_and_return(original_word, expanded_word, 1));
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
