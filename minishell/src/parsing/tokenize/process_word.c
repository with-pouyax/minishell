#include "../../minishell.h"

int	initialize_word(char **word)
{
    *word = ft_strdup("");
    if (!*word)
        return (1);
    return (0);
}

int	should_break(const t_shell_data *shell, char c)
{
    if (!shell->in_single_quote && !shell->in_double_quote)
    {
        if (ft_isspace(c) || is_operator_char(c))
            return (1);
    }
    return (0);
}

void	toggle_quotes(t_shell_data *shell, char c)
{
    if (c == '\'' && !shell->in_double_quote)
        shell->in_single_quote = !shell->in_single_quote;
    else if (c == '\"' && !shell->in_single_quote)
        shell->in_double_quote = !shell->in_double_quote;
}

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

int	finalize_word(char **word)
{
    if (ft_strlen(*word) == 0)
    {
        free(*word);
        *word = NULL;
    }
    return (0);
}

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
            break;
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
