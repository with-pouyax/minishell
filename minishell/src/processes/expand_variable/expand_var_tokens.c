#include "../../minishell.h"

int initialize_expansion(char **result, int *in_single_quote, int *in_double_quote)
{
    *in_single_quote = 0;
    *in_double_quote = 0;
    *result = ft_strdup("");
    if (!*result)
        return (1);
    return (0);
}

void toggle_quotes_and_skip(char current_char, int *in_single_quote, int *in_double_quote, int *i)
{
    if (current_char == '\'' && !(*in_double_quote))
        *in_single_quote = !(*in_single_quote);
    else if (current_char == '\"' && !(*in_single_quote))
        *in_double_quote = !(*in_double_quote);
    (*i)++;
}








int cleanup_and_return_null(char *result)
{
    free(result);
    return (1);
}



//=======================

static int	append_str_to_result(char **result, char *str)
{
	char	*temp;

	temp = ft_strjoin_free_both(*result, str);
	if (!temp)
		return (1);
	*result = temp;
	return (0);
}

/*
** Handle $? expansion: convert exit_status to string and append to result.
** Returns 1 on error, 0 on success.
*/
static int	handle_exit_status(t_shell_data *shell, char **result)
{
	char	*exit_str;

	exit_str = ft_itoa(shell->exit_status);
	if (!exit_str)
		return (1);
	if (append_str_to_result(result, exit_str))
		return (1);
	return (0);
}

/*
** Handle $VAR expansion: extract variable name, get its value, append to result.
** i points to the character after '$' on entry.
** Returns 1 on error, 0 on success.
*/
static int	handle_variable_expansion(t_shell_data *shell, char *input, int *i, char **result)
{
	char	*var_value;
	int		var_not_found_flag;

	var_not_found_flag = 0;
	var_value = get_variable_value(shell, input, i, &var_not_found_flag);
	if (!var_value)
		return (1);
	/* var_value will be "" if not found, so just append it (no error). */
	if (append_str_to_result(result, var_value))
		return (1);
	return (0);
}

/*
** Handle cases where $ is followed by a digit or invalid variable name:
** We skip the invalid characters (if any), resulting in empty expansion.
** Example: $9abc -> empty, just move i past '9abc'.
** Returns 0 always since no error, just skip var chars.
*/
static void	skip_invalid_var_chars(char *input, int *i)
{
	int	var_len;

	var_len = get_var_name_len(&input[*i]);
	*i += var_len;
	/* Empty expansion appended (no var), do nothing. */
}

/*
** Handle $ expansions. If no valid variable follows, append literal '$' if needed.
** Called only when we see '$' and we are not in single quotes.
** Returns 1 on error, 0 on success.
*/
static int	handle_dollar(t_shell_data *shell, char *input, int *i, char **result)
{
	(*i)++; /* Skip '$' */
	if (input[*i] == '?')
	{
		(*i)++;
		if (handle_exit_status(shell, result))
			return (1);
	}
	else if (ft_isalpha((unsigned char)input[*i]) || input[*i] == '_')
	{
		if (handle_variable_expansion(shell, input, i, result))
			return (1);
	}
	else if (ft_isdigit((unsigned char)input[*i]))
	{
		skip_invalid_var_chars(input, i);
		/* Expands to empty string: do nothing */
	}
	else
	{
		/*
		** No valid var name after '$'. Treat '$' as literal.
		** We already advanced i by one, so '$' is consumed.
		** Let's just append '$' literally if we want to represent it.
		*/
		{
			//int tmp_i = 0;
			char *temp = ft_strdup("$");
			if (!temp)
				return (1);
			if (append_str_to_result(result, temp))
				return (1);
		}
	}
	return (0);
}



char	*expand_variables_in_token(t_shell_data *shell, char *input)
{
    char	*result;
    int		i;
    int		in_single_quote;
    int		in_double_quote;

    if (initialize_expansion(&result, &in_single_quote, &in_double_quote))                                  //we intialize everything for the expansion of the variables in the token
        return (NULL);
    i = 0;
    while (input[i])
    {
        if ((input[i] == '\'' && !in_double_quote) || (input[i] == '\"' && !in_single_quote))               //if we reach a single quote and we are not in a double quote or if we reach a double quote and we are not in a single quote
            toggle_quotes_and_skip(input[i], &in_single_quote, &in_double_quote, &i);                       //we reverse the quote flags and skip the character
    else if (input[i] == '$' && !in_single_quote)
		{
			/* Handle variable expansion */
			if (handle_dollar(shell, input, &i, &result))
			{
				free(result);
				return (NULL);
			}
		}
		else
		{
			/* Append literal character */
			if (append_literal_char(input, &i, &result))
			{
				free(result);
				return (NULL);
			}
		}
	}
	return (result);
}
