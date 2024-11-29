#include "../../minishell.h"

char	*expand_variables_in_token(t_shell_data *shell, char *input,
					int *var_not_found_flag)
{
	char	*result;
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (input[i])
	{
		update_quote_flags(input[i], &in_single_quote, &in_double_quote);
		if (input[i] == '$' && !in_single_quote)
		{
			if (process_variable_expansion(shell, input, &i,
					&result, var_not_found_flag))
				return (NULL);
		}
		else
		{
			if (append_literal_char(input, &i, &result))
				return (NULL);
		}
	}
	return (result);
}
