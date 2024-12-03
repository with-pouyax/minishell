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
    if (!shell->in_single_quote && !shell->in_double_quote)
    {
        if (ft_isspace(c) || is_operator_char(c))
            return (1);
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
    if ((c == '\'' && !shell->in_double_quote) || (c == '\"' && !shell->in_single_quote))
    {
        toggle_quotes(shell, c);
    }
    if (add_current_char(shell, c, word))
        return (1);
    return (0);
}

// Refactored collect_word function
int	collect_word(char *input, int *i, char **word, t_shell_data *shell)
{
    int	ret;

    if (!*word)
    {
        if (initialize_word(word))
            return (1);
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






// Function to process a word token
int process_word(t_shell_data *shell, char *input, int *i, t_command *cmd, int *index)
{
    char    *word = NULL;
    char    *expanded_word;
    char    *original_word;
    int     var_not_found_flag;
    t_token *last_token;

    var_not_found_flag = 0;
    word = ft_strdup("");
    if (!word)
        return (1);
    if (collect_word(input, i, &word, shell))
    {
        if (word)
            free(word);
        return (1);
    }

    // Save the original word before expansion
    original_word = ft_strdup(word);
    if (!original_word)
    {
        free(word);
        return (1);
    }

    // Expand variables in the word
    expanded_word = expand_variables_in_token(shell, word, &var_not_found_flag);
    free(word);
    if (!expanded_word)
    {
        free(original_word);
        return (1);
    }
    word = expanded_word;

    // Add the token with the expanded word
    if (add_token(word, &cmd->token_list, index, 0))
    {
        free(word);
        free(original_word);
        return (1);
    }

    // Set the original_value in the last token added
    last_token = cmd->token_list;
    while (last_token->next)
        last_token = last_token->next;

    last_token->original_value = original_word;

    return (0);
}
