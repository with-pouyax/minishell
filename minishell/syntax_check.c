#include "minishell.h"

int	check_trailing_pipe(char *input)
{
	int	i;

	if (!input)
		return (0);
	i = ft_strlen(input) - 1; // Get the index of the last character in the input string
	while (i >= 0 && ft_isspace(input[i])) // Loop through the input string from the end to find the last non-space character
		i--;
	if (i >= 0 && input[i] == '|') // Check if the last non-space character is a pipe
		return (1);

	return (0);
}
