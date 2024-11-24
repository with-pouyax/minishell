#include "../../minishell.h"

int collect_word(char *input, int *i, char **word)
{
    int ret;
    int in_single_quote = 0;
    int in_double_quote = 0;

    while (input[*i] && (!ft_isspace(input[*i]) || in_single_quote || in_double_quote))
    {
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
                return (1);
        }
    }
    if (in_single_quote || in_double_quote)
    {
        ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR_FILENO);
        return (1);
    }
    return (0);
}

int process_word(t_shell_data *shell, char *input, int *i, t_command *cmd, int *index)
{
    char *word;
    char *expanded_word;
    int var_not_found_flag = 0;

    word = ft_strdup("");
    if (!word)
        return (1);
    if (collect_word(input, i, &word))
    {
        free(word);
        return (1);
    }

    // Expand variables in the collected word
    expanded_word = expand_variables_in_token(shell, word, &var_not_found_flag);
    if (!expanded_word)
    {
        free(word);
        return (1);
    }
    free(word);
    word = expanded_word;

    if (add_token(word, &cmd->token_list, index, 0))
    {
        free(word); // Properly free token_value on failure
        return (1);
    }
    return (0);
}



