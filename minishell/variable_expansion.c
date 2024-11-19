#include "minishell.h"

char	*getenv_from_envp(t_shell_data *shell, char *name)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (shell->envp[i]) // Loop through the environment variables
	{
		if (ft_strncmp(shell->envp[i], name, len) == 0
			&& shell->envp[i][len] == '=') // If the variable name matches the name we are looking for and the character after the variable name is an = character
			return (shell->envp[i] + len + 1); // Return the value of the variable(+1 to skip the = character)
		i++; // Move to the next environment variable
	}
	return (NULL);
}

int	get_var_name_len(t_shell_data *shell, char *str)
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

char	*expand_variable_token(t_shell_data *shell, char *input, int *i, int *var_not_found_flag)
{
	char	*var_value;

	if (input[*i] == '?')
	{
		(*i)++; // Skip the ? character
		var_value = ft_itoa(shell->exit_status); // Get the exit status from the global data struct and convert it to a string and store it in var_value
	}
	else if (input[*i] == ' ' || input[*i] == '=' || input[*i] == '+')
	{
		var_value = ft_strdup("$"); // If the character is a space, =, or + character, set var_value to a $ character
		(*var_not_found_flag) = 1; // Set the var_not_found_flag to true
	}
	else 
		var_value = get_variable_value(shell, input, i, var_not_found_flag); //ok
	return (var_value); // Return the value of the variable
}

char	*get_variable_value(t_shell_data *shell, char *input, int *i, int *var_not_found_flag)
{
	char	*var_name;
	char	*var_value;
	int		var_len;

	var_len = get_var_name_len(shell, &input[*i]); //ok
	var_name = ft_substr(input, *i, var_len); //ok
	if (!var_name)
		return (NULL);
	var_value = getenv_from_envp(shell, var_name); //ok
	if (var_value) // If the variable is found in the environment variables
		var_value = ft_strdup(var_value); // ok
	else // If the variable is not found
	{
		var_value = ft_strdup(""); // ok
		*var_not_found_flag = 1; // Set the var_not_found_flag to true
	}
	*i += var_len; // Move the index to the end of the variable name to skip it
	free(var_name); 
	return (var_value); // Return the value of the variable
}

int	process_variable_expansion(t_shell_data *shell, char *input, int *i, char **result, int *flag)
{
	char	*temp;

		(*i)++; // Skip the $ character
	temp = expand_variable_token(shell, input, i, flag); //ok
	if (!temp)
		return (1);
	*result = ft_strjoin_free_both(*result, temp); //ok
	if (!*result) // If there is an error
		return (1); 
	return (0);
}
int	append_literal_char(char *input, int *i, char **result)
{
	char	*temp;

	temp = get_literal_char(input, i); //ok
	if (!temp)
		return (1);
	*result = ft_strjoin_free_both(*result, temp); //ok
	if (!*result)
		return (1);
	return (0);
}

char	*expand_variables_in_token(t_shell_data *shell, char *input, int *var_not_found_flag)
{
	char	*result;
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	result = ft_strdup(""); // ok
	if (!result)
		return (NULL);
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (input[i]) // Loop through the input string
	{
		update_quote_flags(input[i], &in_single_quote, &in_double_quote); //ok 
		if (input[i] == '$' && !in_single_quote) // If we encounter a $ character and we are not in a single quote
		{
			if (process_variable_expansion(shell, input, &i, &result, var_not_found_flag)) //ok
				return (NULL);
		}
		else // If we encounter a character that is not a $ character
		{
			if (append_literal_char(input, &i, &result)) //ok
				return (NULL);
		}
	}
	return (result); // Return the result
}

void	update_quote_flags(char c, int *in_single_quote, int *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote)) // If the character is a single quote and we are not in a double quote
		*in_single_quote = !(*in_single_quote); // reverse the value of in_single_quote
	else if (c == '\"' && !(*in_single_quote)) // If the character is a double quote and we are not in a single quote
		*in_double_quote = !(*in_double_quote); // reverse the value of in_double_quote
}
void expand_variables_in_input(t_shell_data *shell)
{
    char *expanded_input;
    int var_not_found_flag = 0;

    expanded_input = expand_variables_in_token(shell, shell->input, &var_not_found_flag); //ok
    if (!expanded_input)
    {
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
        free(shell->input);
        shell->input = NULL;
        shell->exit_status = 1;
        return;
    }
    free(shell->input);
    shell->input = expanded_input;
}

void expand_variables_in_tokens(void)
{
    t_command *cmd;
    t_token *token;
    char *expanded_value;
    int var_not_found_flag;

    cmd = g_data.commands;
    while (cmd)
    {
        token = cmd->token_list;
        while (token)
        {
            if (!token->is_operator)
            {
                var_not_found_flag = 0;

                // Store the original value before expansion
                token->original_value = ft_strdup(token->value);
                if (!token->original_value)
                {
                    ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
                    g_data.exit_status = 1;
                    // Handle the error appropriately (e.g., set an error flag, clean up, etc.)
                    return;
                }

                // Expand the variable
                expanded_value = expand_variables_in_token(token->value, &var_not_found_flag);
                if (!expanded_value)
                {
                    ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
                    free(token->original_value);
                    token->original_value = NULL;
                    g_data.exit_status = 1;
                    // Handle the error appropriately
                    return;
                }

                // Replace the token value with the expanded value
                free(token->value);
                token->value = expanded_value;
                token->var_not_found = var_not_found_flag;
            }
            token = token->next;
        }
        cmd = cmd->next;
    }
}

