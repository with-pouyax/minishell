// syntax_check.c
#include "minishell.h"

/**
 * @brief Checks if the input string ends with a pipe '|', ignoring trailing whitespace.
 *
 * @param input The input command string.
 * @return int Returns 1 if there's a trailing pipe, 0 otherwise.
 */
int	check_trailing_pipe(char *input)
{
	int	i;

	if (!input)
		return (0);

	i = ft_strlen(input) - 1;

	// Skip trailing whitespace
	while (i >= 0 && ft_isspace(input[i]))
		i--;

	// Check if the last non-space character is '|'
	if (i >= 0 && input[i] == '|')
		return (1);

	return (0);
}
