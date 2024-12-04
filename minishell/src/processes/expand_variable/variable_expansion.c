#include "../../minishell.h"

/*
shell->envp[i][len] == '=') // If the variable name matches the
name we are looking for and the character after the variable name
is an = character return (shell->envp[i] + len + 1); // Return the
value of the variable(+1 to skip the = character)
*/
char	*getenv_from_envp(t_shell_data *shell, char *name)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], name, len) == 0
			&& shell->envp[i][len] == '=')
			return (shell->envp[i] + len + 1);
		i++;
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

/*
str = ft_substr(input, *i, 1); // Get the character at the current
index and store it in str (*i)++; // Move the index to the next character
return (str); // Return the character
*/
char	*get_literal_char(char *input, int *i)
{
	char	*str;

	str = ft_substr(input, *i, 1);
	(*i)++;
	return (str);
}

/*
(*i)++;// Skip the $ character
*result = ft_strjoin_free_both(*result, temp); 
      // Join the result and the expanded variable and store the result in result
*/
int	process_variable_expansion(t_shell_data *shell, char *input, int *i,
				char **result)
{
	char	*temp;

	(*i)++;
	temp = expand_variable_token(shell, input, i, &shell->var_not_found_flag);
	if (!temp)
		return (1);
	*result = ft_strjoin_free_both(*result, temp);
	if (!*result)
		return (1);
	return (0);
}

/*
 (literal characters are characters that are not $ characters and are not part
 of a variable)
*/
int	append_literal_char(char *input, int *i, char **result)
{
	char	*temp;

	temp = get_literal_char(input, i);
	if (!temp)
		return (1);
	*result = ft_strjoin_free_both(*result, temp);
	if (!*result)
		return (1);
	return (0);
}
