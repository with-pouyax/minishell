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
	(void)input;
	str = ft_substr(input, *i, 1);
	if (!str)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
	}
	(*i)++;
	return (str);
}



/*
 (literal characters are characters that are not $ characters and are not part
 of a variable)
*/
int	append_literal_char(char *input, int *i, char **result)
{
	char	*temp;

	temp = get_literal_char(input, i); //[x]
	if (!temp)
		return (1);
	*result = ft_strjoin_free_both(*result, temp); //[x]
	if (!*result)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}
