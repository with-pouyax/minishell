#include "../minishell.h"


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
	if (delimiter[ft_strlen(delimiter) - 1] == '\'' || delimiter[ft_strlen(delimiter) - 1] == '\"')
	{
		while (delimiter[ft_strlen(delimiter) - 1] == '\'' || delimiter[ft_strlen(delimiter) - 1] == '\"')
			delimiter[ft_strlen(delimiter) - 1] = '\0';
		unquoted_delimiter = ft_substr(delimiter, 0, ft_strlen(delimiter));
		if (!unquoted_delimiter)
			return (-1);
		free(redir->delimiter);
		redir->delimiter = unquoted_delimiter;
		return (1);
	}
	return (0);
}
