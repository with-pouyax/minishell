#include "../minishell.h"

/*
if (input[i] == '\'' && !in_double_quote)-> if single q and not in double q
in_single_quote = !in_single_quote; -> reverse the value of in_single_q
else if (input[i] == '\"' && !in_single_quote)-> if double q and not in single q
in_double_quote = !in_double_quote;-> reverse the value of in_double_q
return (in_single_quote || in_double_quote);-> Return 1 if any of the q 
						are 1, 0 otherwise.
*/
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
		if (input[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (input[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	return (in_single_quote || in_double_quote);
}

/*
if (c == '\'' && !(*in_double_quote))-> If the char is a single q
and we are not in a double q
*in_single_quote = !(*in_single_quote)-> reverse the value of in_single_q
else if (c == '\"' && !(*in_single_quote))->If the char is a double q and
we are not in a single q
*in_double_quote = !(*in_double_quote)-> reverse the value of in_double_q
*/
void	update_quote_flags(char c, int *in_single_quote, int *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
		*in_single_quote = !(*in_single_quote);
	else if (c == '\"' && !(*in_single_quote))
		*in_double_quote = !(*in_double_quote);
}
