#include "minishell.h"

char	*getenv_from_envp(char *name)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (g_data.envp[i]) // Loop through the environment variables
	{
		if (ft_strncmp(g_data.envp[i], name, len) == 0
			&& g_data.envp[i][len] == '=') // If the variable name matches the name we are looking for and the character after the variable name is an = character
			return (g_data.envp[i] + len + 1); // Return the value of the variable(+1 to skip the = character)
		i++; // Move to the next environment variable
	}
	return (NULL);
}

int	get_var_name_len(char *str)
{
	int	len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}

char	*get_literal_char(char *input, int *i)
{
	char	*str;

	str = ft_substr(input, *i, 1); // Get the character at the current index and store it in str
	(*i)++; // Move the index to the next character
	return (str); // Return the character
}

char	*expand_variable_token(char *input, int *i, int *var_not_found_flag)
{
	char	*var_value;

	if (input[*i] == '?')
	{
		(*i)++; // Skip the ? character
		var_value = ft_itoa(g_data.exit_status); // Get the exit status from the global data struct and convert it to a string and store it in var_value
	}
	else // If the character is not a ? character 
		var_value = get_variable_value(input, i, var_not_found_flag); // Get the value of the variable and store it in var_value
	return (var_value); // Return the value of the variable
}

char	*get_variable_value(char *input, int *i, int *var_not_found_flag)
{
	char	*var_name;
	char	*var_value;
	int		var_len;

	var_len = get_var_name_len(&input[*i]); // Get the length of the variable name, we will use this to extract the variable name from the input string
	var_name = ft_substr(input, *i, var_len); // Extract the variable name from the input string and store it in var_name
	if (!var_name)
		return (NULL);
	var_value = getenv_from_envp(var_name); // Get the value of the variable from the environment variables
	if (var_value) // If the variable is found in the environment variables
		var_value = ft_strdup(var_value); // Copy the value of the variable to var_value
	else // If the variable is not found
	{
		var_value = ft_strdup(""); // Set var_value to an empty string
		*var_not_found_flag = 1; // Set the var_not_found_flag to true
	}
	*i += var_len; // Move the index to the end of the variable name to skip it
	free(var_name); 
	return (var_value); // Return the value of the variable
}

int	process_variable_expansion(char *input, int *i, char **result, int *flag)
{
	char	*temp;

	(*i)++; // Skip the $ character
	temp = expand_variable_token(input, i, flag); // Expand the variable and store the result in temp
	if (!temp)
		return (1);
	*result = ft_strjoin_free_both(*result, temp); // Join the result and the expanded variable and store the result in result
	if (!*result) // If there is an error
		return (1); 
	return (0);
}
int	append_literal_char(char *input, int *i, char **result)
{
	char	*temp;

	temp = get_literal_char(input, i); // Get the literal character and store it in temp (literal characters are characters that are not $ characters and are not part of a variable)
	if (!temp)
		return (1);
	*result = ft_strjoin_free_both(*result, temp); // Join the result and the literal character and store the result in result
	if (!*result)
		return (1);
	return (0);
}

char	*expand_variables_in_token(char *input, int *var_not_found_flag)
{
	char	*result;
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	result = ft_strdup(""); // Allocate memory for the result
	if (!result)
		return (NULL);
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (input[i]) // Loop through the input string
	{
		update_quote_flags(input[i], &in_single_quote, &in_double_quote);
		if (input[i] == '$' && !in_single_quote) // If we encounter a $ character and we are not in a single quote
		{
			if (process_variable_expansion(input, &i, &result, var_not_found_flag)) // Process the variable expansion
				return (NULL);
		}
		else // If we encounter a character that is not a $ character
		{
			if (append_literal_char(input, &i, &result)) // Append the character to the result
				return (NULL);
		}
	}
	return (result); // Return the result
}

void	update_quote_flags(char c, int *in_single_quote, int *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
		*in_single_quote = !(*in_single_quote);
	else if (c == '\"' && !(*in_single_quote))
		*in_double_quote = !(*in_double_quote);
}

void	expand_variables_in_tokens(void)
{
	t_command	*cmd;
	t_token		*token;
	char		*expanded_value;
	int			var_not_found_flag;

	cmd = g_data.commands;
	while (cmd)
	{
		token = cmd->token_list;
		while (token)
		{
			if (!token->is_operator)
			{
				var_not_found_flag = 0;
				expanded_value = expand_variables_in_token(token->value,
						&var_not_found_flag);
				free(token->value);
				token->value = expanded_value;
				token->var_not_found = var_not_found_flag;
			}
			token = token->next;
		}
		cmd = cmd->next;
	}
}