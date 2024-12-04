#include "../minishell.h"

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
