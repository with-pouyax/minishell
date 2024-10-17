#include "minishell.h"

char *get_word(char *input, int *i)
{
    int     start;
    char    *word;

    start = *i;
    while (input[*i] && !is_whitespace(input[*i]) && !is_special_char(input[*i]) &&
           input[*i] != '\'' && input[*i] != '\"')
    {
        (*i)++;
    }
    word = ft_substr(input, start, *i - start);
    return (word);
}


char *get_quoted_word(char *input, int *i, char quote_char)
{
    int     start;
    char    *word;
    char    *temp;

    start = ++(*i);
    while (input[*i] && input[*i] != quote_char)
    {
        if (input[*i] == '\\' && quote_char == '\"' && (input[*i + 1] == '$' || input[*i + 1] == '`' || input[*i + 1] == '\"' || input[*i + 1] == '\\'))
            (*i)++; // Skip backslash in double quotes
        (*i)++;
    }
    if (input[*i] != quote_char)
    {
        ft_putendl_fd("minishell: syntax error: unclosed quote", STDERR_FILENO);
        return (NULL);
    }
    word = ft_substr(input, start, *i - start);
    (*i)++;
    if (quote_char == '\"')
    {
        temp = expand_variables_in_word(word);
        free(word);
        word = temp;
    }
    return (word);
}

