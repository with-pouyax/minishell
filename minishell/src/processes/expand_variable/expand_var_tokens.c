#include "../../minishell.h"

int initialize_expansion(char **result, int *in_single_quote, int *in_double_quote)
{
    *in_single_quote = 0;
    *in_double_quote = 0;
    *result = ft_strdup("");
    if (!*result)
        return (1);
    return (0);
}

void toggle_quotes_and_skip(char current_char, int *in_single_quote, int *in_double_quote, int *i)
{
    if (current_char == '\'' && !(*in_double_quote))
        *in_single_quote = !(*in_single_quote);
    else if (current_char == '\"' && !(*in_single_quote))
        *in_double_quote = !(*in_double_quote);
    (*i)++;
}

int handle_variable_expansion(t_shell_data *shell, char *input, int *i, char **result)
{
    if (process_variable_expansion(shell, input, i, result))                             //we process the variable expansion
    {
        free(*result);
        return (1);
    }
    return (0);
}



int handle_literal_character(char *input, int *i, char **result)
{
    if (append_literal_char(input, i, result))
    {
        free(*result);
        return (1);
    }
    return (0);
}


int cleanup_and_return_null(char *result)
{
    free(result);
    return (1);
}




char	*expand_variables_in_token(t_shell_data *shell, char *input)
{
    char	*result;
    int		i;
    int		in_single_quote;
    int		in_double_quote;

    if (initialize_expansion(&result, &in_single_quote, &in_double_quote))                                  //we intialize everything for the expansion of the variables in the token
        return (NULL);
    i = 0;
    while (input[i])
    {
        if ((input[i] == '\'' && !in_double_quote) || (input[i] == '\"' && !in_single_quote))               //if we reach a single quote and we are not in a double quote or if we reach a double quote and we are not in a single quote
            toggle_quotes_and_skip(input[i], &in_single_quote, &in_double_quote, &i);                       //we reverse the quote flags and skip the character
        else if (input[i] == '$' && !in_single_quote)                                                       //if we reach a dollar sign and we are not in a single quote
        {
            if (handle_variable_expansion(shell, input, &i, &result))                                       //we handle the variable expansion
                return (NULL);
        }
        else
        {
            if (handle_literal_character(input, &i, &result))
                return (NULL);
        }
    }
    return (result);
}
