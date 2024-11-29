#include "../minishell.h"

int	check_leading_pipe(char *input)
{
	int	i;

	if (!input)
		return (0);
	i = 0;
	while (input[i] && ft_isspace(input[i]))
		i++;
	if (input[i] == '|')
		return (1);
	return (0);
}

int	check_trailing_pipe(char *input)
{
	int	i;

	if (!input)
		return (0);
	i = ft_strlen(input) - 1;
	while (i >= 0 && ft_isspace(input[i]))
		i--;
	if (i >= 0 && input[i] == '|')
		return (1);
	return (0);
}
