#include "../minishell.h"


int	check_delimiter_quotes(t_redirection *redir)
{
	char	*delimiter;
	char	*unquoted_delimiter;

	delimiter = redir->delimiter;
	if (delimiter[0] == '\'' || delimiter[0] == '\"')  // if the first character of the delimiter is a quote, we remove it
	{
		unquoted_delimiter = ft_substr(delimiter, 1, ft_strlen(delimiter) - 2); //[x]
		if (!unquoted_delimiter)
			return (-1);
		free(redir->delimiter);
		redir->delimiter = unquoted_delimiter;
		return (1);
	}
	// if (delimiter[ft_strlen(delimiter) - 1] == '\'' || delimiter[ft_strlen(delimiter) - 1] == '\"') //if the last character of the delimiter is a quote, we remove it
	// {
	// 	while (delimiter[ft_strlen(delimiter) - 1] == '\'' || delimiter[ft_strlen(delimiter) - 1] == '\"')
	// 		delimiter[ft_strlen(delimiter) - 1] = '\0';
	// 	unquoted_delimiter = NULL;//ft_substr(delimiter, 0, ft_strlen(delimiter));
	// 	//printf("I am here\n");
	// 	if (!unquoted_delimiter)
	// 		return (-1);
	// 	free(redir->delimiter);
	// 	redir->delimiter = unquoted_delimiter;
	// 	return (1);
	// }
	return (0);
}
