#include "../../minishell.h"







int collect_word(char *input, int *i, char **word)
{
    int ret;
    int in_single_quote = 0;
    int in_double_quote = 0;

    *word = ft_strdup("");
    if (!*word)
        return (1);

    while (input[*i])
    {
        if (!in_single_quote && !in_double_quote)
        {
            if (ft_isspace(input[*i]))
                break;
            if (is_operator_char(input[*i]))
                break;
        }

        if (input[*i] == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            (*i)++;
        }
        else if (input[*i] == '\"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            (*i)++;
        }
        else
        {
            ret = add_char_to_token(word, input[*i]);
            (*i)++;
            if (ret)
            {
                free(*word);
                return (1);
            }
        }
    }

    if (in_single_quote || in_double_quote)
    {
        ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR_FILENO);
        free(*word);
        return (1);
    }

    if (ft_strlen(*word) == 0)
    {
        free(*word);
        *word = NULL;
    }

    return (0);
}




// process_word.c or a relevant file
#include "../../minishell.h"

// Function to process a word token
int process_word(t_shell_data *shell, char *input, int *i, t_command *cmd, int *index)
{
    char    *word;
    char    *expanded_word;
    char    *original_word;
    int     var_not_found_flag;
    t_token *last_token;

    var_not_found_flag = 0;
    word = ft_strdup("");
    if (!word)
        return (1);
    if (collect_word(input, i, &word))
    {
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
