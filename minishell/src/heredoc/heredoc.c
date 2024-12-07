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

int expand_and_write_line(t_shell_data *shell ,char *line, int fd)
{
    char    *temp;

    temp = expand_variables_in_token(shell, line); // Expand the variables in the line
    if (!temp)
        return (1);
    write(fd, temp, ft_strlen(temp)); // Write the expanded line to the file
    write(fd, "\n", 1); // Write a newline character to the file
    free(temp); // Free the expanded line
    return (0);
}




