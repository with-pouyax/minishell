#include "minishell.h"

// Internal command check function
int is_internal_command(char *token)
{
    return (ft_strcmp(token, "echo") == 0 || ft_strcmp(token, "cd") == 0 ||
            ft_strcmp(token, "pwd") == 0 || ft_strcmp(token, "export") == 0 ||
            ft_strcmp(token, "unset") == 0 || ft_strcmp(token, "env") == 0 ||
            ft_strcmp(token, "exit") == 0);
}

// Helper function to check if a token is an operator
int is_operator(char *token)
{
    return (ft_strcmp(token, ">") == 0 || ft_strcmp(token, ">>") == 0 ||
            ft_strcmp(token, "<") == 0 || ft_strcmp(token, "<<") == 0 ||
            ft_strcmp(token, "|") == 0);
}

// Helper function to check for unclosed quotes
int check_unclosed_quotes(char *input)
{
    int in_single_quote = 0;
    int in_double_quote = 0;

    while (*input)
    {
        if (*input == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        else if (*input == '\"' && !in_single_quote)
            in_double_quote = !in_double_quote;
        input++;
    }
    return (in_single_quote || in_double_quote);
}
