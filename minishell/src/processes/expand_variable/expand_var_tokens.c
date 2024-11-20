#include "minishell.h"


char	*expand_variables_in_token(char *input, int *var_not_found_flag)
{
	char	*result;
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	result = ft_strdup("");           // Allocate memory for the result
	if (!result)
		return (NULL);
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (input[i])                  // Loop through the input string
	{
		update_quote_flags(input[i], &in_single_quote, &in_double_quote);            // Update the quote flags based on the current character
		if (input[i] == '$' && !in_single_quote)                                     // If we encounter a $ character and we are not in a single quote
		{
			if (process_variable_expansion(input, &i, &result, var_not_found_flag)) // Process the variable expansion
				return (NULL);
		}
		else // If we encounter a character that is not a $ character
		{
			if (append_literal_char(input, &i, &result)) // Append the character to the result, so after the result will our input string with the variables expanded
				return (NULL);
		}
	}
	return (result); // Return the result
}
