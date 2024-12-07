#include "../minishell.h"



/*
check_delimiter_quotes
- Checks if the heredoc delimiter is quoted.
- Removes the quotes if they exist, ensuring that special character
expansions (like variables) are handled appropriately.
*/
int	check_delimiter_quotes(t_redirection *redir)
{
	char	*delimiter;
	char	*unquoted_delimiter;

	delimiter = redir->delimiter;
	if (delimiter[0] == '\'' || delimiter[0] == '\"')
	{
		unquoted_delimiter = ft_substr(delimiter, 1, ft_strlen(delimiter) - 2);
		if (!unquoted_delimiter)
			return (-1);
		free(redir->delimiter);
		redir->delimiter = unquoted_delimiter;
		return (1);
	}
	return (0);
}
