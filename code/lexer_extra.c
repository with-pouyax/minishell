#include "minishell.h"

char    *get_word(char *input, int *i)
{
    int     start;
    char    *word;

    start = *i;
    while (input[*i] && !is_whitespace(input[*i]) && !is_special_char(input[*i]))
        (*i)++;
    word = ft_substr(input, start, *i - start);
    return (word);
}

char    *get_quoted_word(char *input, int *i)
{
    int     start;
    char    quote;
    char    *word;

    quote = input[*i];
    start = ++(*i);
    while (input[*i] && input[*i] != quote)
        (*i)++;
    if (input[*i] != quote)
    {
        ft_putendl_fd("minishell: syntax error: unclosed quote", STDERR_FILENO);
        return (NULL);
    }
    word = ft_substr(input, start, *i - start);
    (*i)++;
    return (word);
}
