#include "../minishell.h"



char	*get_variable_value(t_shell_data *shell, char *input, int *i, int *var_not_found_flag)
{
	char	*var_name;
	char	*var_value;
	int		var_len;

	var_len = get_var_name_len(&input[*i]); // Get the length of the variable name, we will use this to extract the variable name from the input string
	var_name = ft_substr(input, *i, var_len); // Extract the variable name from the input string and store it in var_name
	if (!var_name)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (NULL);
	}
	var_value = getenv_from_envp(shell, var_name); // Get the value of the variable from the environment variables
	if (var_value) // If the variable is found in the environment variables
		var_value = ft_strdup(var_value); // Copy the value of the variable to var_value
	else // If the variable is not found
	{
		var_value = ft_strdup(""); // Set var_value to an empty string
		if (!var_value)
		{
			ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		}
		*var_not_found_flag = 1; // Set the var_not_found_flag to true
	}
	*i += var_len; // Move the index to the end of the variable name to skip it
	free(var_name); 
	return (var_value); // Return the value of the variable
}

