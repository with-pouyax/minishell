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

static int	handle_variable_expansion(t_shell_data *shell, char *input, int *i, char **result)
{
	char	*var_value;
	int		var_not_found_flag;

	var_not_found_flag = 0;
	var_value = get_variable_value(shell, input, i, &var_not_found_flag);
	if (!var_value)
		return (1);
	if (append_str_to_result(result, var_value))
		return (1);
	return (0);
}

int	handle_dollar(t_shell_data *shell, char *input, int *i, char **result)
{
	(*i)++; // Skip the '$' character

	if (input[*i] == '?')
	{
		(*i)++; // Skip the '?' character
		if (handle_exit_status(shell, result))
			return (1);
	}
	else if (ft_isalpha((unsigned char)input[*i]) || input[*i] == '_')
	{
		if (handle_variable_expansion(shell, input, i, result))
			return (1);
	}
	else if (ft_isdigit((unsigned char)input[*i]))
		(*i)++;
	else
	{
		char *temp = ft_strdup("$");
		if (!temp)
			return (1);
		if (append_str_to_result(result, temp))
			return (free(temp), 1);
	}
	return (0);
}

char	*expand_variables_in_token(t_shell_data *shell, char *input)
{
	char	*result;
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	if (initialize_expansion(&result, &in_single_quote, &in_double_quote))
		return (NULL);
	i = 0;
	while (input[i])
	{
		if ((input[i] == '\'' && !in_double_quote) ||
			(input[i] == '\"' && !in_single_quote))
			toggle_quotes_and_skip(input[i], &in_single_quote, &in_double_quote, &i);
		else if (input[i] == '$' && !in_single_quote)
		{
			shell->expanded = 1;
			if (handle_dollar(shell, input, &i, &result))
				return (free(result), NULL);
		}
		else
		{
			if (append_literal_char(input, &i, &result))
				return (free(result), NULL);
		}
	}
	return (result);
}
