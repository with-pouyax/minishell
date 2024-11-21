#include "../minishell.h"


int	check_unclosed_quotes(char *input)
{
	int	in_single_quote;
	int	in_double_quote;
	int	i;

	in_single_quote = 0;
	in_double_quote = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quote)             // if single quote and not in double quote
			in_single_quote = !in_single_quote;               // reverse the value of in_single_quote
		else if (input[i] == '\"' && !in_single_quote)        // if double quote and not in single quote
			in_double_quote = !in_double_quote;               // reverse the value of in_double_quote
		i++;
	}
	return (in_single_quote || in_double_quote);               // Return 1 if any of the quotes are 1, 0 otherwise.
}

void	update_quote_flags(char c, int *in_single_quote, int *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))                       // If the character is a single quote and we are not in a double quote
		*in_single_quote = !(*in_single_quote);                  // reverse the value of in_single_quote
	else if (c == '\"' && !(*in_single_quote))                  // If the character is a double quote and we are not in a single quote
		*in_double_quote = !(*in_double_quote);                 // reverse the value of in_double_quote
}

