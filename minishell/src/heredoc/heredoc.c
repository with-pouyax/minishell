#include "../minishell.h"



void	skip_until_operator_or_space(char *input, int *i)
{
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator_char(input[*i]))
		(*i)++;
}

char *get_line_from_input(char *input, int *index)
{
    int start = *index;
    while (input[*index] && input[*index] != '\n')
        (*index)++;
    char *line = ft_substr(input, start, *index - start);
    if (input[*index] == '\n')
        (*index)++;
    return line;
}
/*
expand_and_write_line
- Expands variables (e.g., $VAR) in a line of heredoc input.
- Write
*/

int toggle_quotes_and_append(char current_char, int *in_single_quote, int *in_double_quote, int *i, char **result)
{
    if (current_char == '\'' && !(*in_double_quote))
        *in_single_quote = !(*in_single_quote);
    else if (current_char == '\"' && !(*in_single_quote))
        *in_double_quote = !(*in_double_quote);

    // Append the quote character to the result
    if (add_char_to_token(result, current_char))
        return (1); // Indicate failure if memory allocation fails

    (*i)++;
    return (0); // Success
}

char	*expand_variables_in_heredoc(t_shell_data *shell, char *input)
{
    char	*result;
    int		i;
    int		in_single_quote;
    int		in_double_quote;

    if (initialize_expansion(&result, &in_single_quote, &in_double_quote))
        return (NULL);
    i = 0;
    while (input[i])
    {
        if ((input[i] == '\'' && !in_double_quote) ||
            (input[i] == '\"' && !in_single_quote))
        {
            if (toggle_quotes_and_append(input[i], &in_single_quote, &in_double_quote, &i, &result))
                return (free(result), NULL);
        }
        else if (input[i] == '$' && !in_single_quote)
        {
            if (handle_dollar(shell, input, &i, &result))
                return (free(result), NULL);
        }
        else
        {
            if (append_literal_char(input, &i, &result))
                return (free(result), NULL);
        }
    }
    return (result);
}

int expand_and_write_line(t_shell_data *shell ,char *line, int fd)
{
    char    *temp;

    temp = expand_variables_in_heredoc(shell, line); // Expand the variables in the line
    if (!temp)
        return (1);
    write(fd, temp, ft_strlen(temp)); // Write the expanded line to the file
    write(fd, "\n", 1); // Write a newline character to the file
    free(temp); // Free the expanded line
    return (0);
}




