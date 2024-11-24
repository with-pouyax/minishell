#include "../minishell.h"

int	get_heredoc_delimiter(char *input, int *i, t_token *heredoc_token)
{
	char	*delimiter;
	int		start;

	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	if (!input[*i])
		return (syntax_error_newline());
	start = *i;
	if (input[*i] == '\'' || input[*i] == '\"')
	{
		if (process_quoted_delimiter(input, i) == -1)
			return (1);
	}
	else
		skip_until_operator_or_space(input, i);
	delimiter = ft_substr(input, start, *i - start);
	if (!delimiter)
		return (1);
	heredoc_token->heredoc_delimiter = delimiter;
	return (0);
}

/*
check_delimiter_quotes
- Checks if the heredoc delimiter is quoted.
- Removes the quotes if they exist, ensuring that special character expansions (like variables) are handled appropriately.

*/

int	check_delimiter_quotes(t_token *heredoc_token)
{
	char	*delimiter;
	char	*unquoted_delimiter;

	delimiter = heredoc_token->heredoc_delimiter;
	if (delimiter[0] == '\'' || delimiter[0] == '\"')
	{
		unquoted_delimiter = ft_substr(delimiter, 1, ft_strlen(delimiter) - 2);
		if (!unquoted_delimiter)
			return (-1);
		free(heredoc_token->heredoc_delimiter);
		heredoc_token->heredoc_delimiter = unquoted_delimiter;
		return (1);
	}
	return (0);
}