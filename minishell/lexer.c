#include "minishell.h"

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_valid_operator(char *op)
{
	if (!ft_strcmp(op, "|") || !ft_strcmp(op, "<")
		|| !ft_strcmp(op, ">") || !ft_strcmp(op, ">>")
		|| !ft_strcmp(op, "<<"))
		return (1);
	return (0);
}

int	add_char_to_token(char **token, char c)
{
	char	tmp[2];
	char	*new_token;

	tmp[0] = c;
	tmp[1] = '\0';
	new_token = ft_strjoin_safe(*token, tmp); //ok
	if (!new_token)
	{
		free(*token);
		*token = NULL;
		return (1);
	}
	free(*token);
	*token = new_token;
	return (0);
}

int	skip_quotes(char *input, int i)
{
	char	quote;

	quote = input[i++];
	while (input[i] && input[i] != quote)
		i++;
	if (input[i] == quote)
		i++;
	return (i);
}
