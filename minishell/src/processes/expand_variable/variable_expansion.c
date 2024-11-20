#include "../../minishell.h"

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

int	process_variable_expansion(t_shell_data *shell, char *input, int *i, char **result, int *flag)
{
	char	*temp;

		(*i)++; // Skip the $ character
	temp = expand_variable_token(shell, input, i, flag); // Expand the variable and store the result in temp
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



