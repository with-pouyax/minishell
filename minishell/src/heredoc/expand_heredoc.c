#include "../minishell.h"

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
		var_value = get_variable_value(shell, input, i, var_not_found_flag); // Get the value of the variable and store it in var_value
	return (var_value); // Return the value of the variable
}

char	*get_variable_value(t_shell_data *shell, char *input, int *i, int *var_not_found_flag)
{
	char	*var_name;
	char	*var_value;
	int		var_len;

	var_len = get_var_name_len(&input[*i]); // Get the length of the variable name, we will use this to extract the variable name from the input string
	var_name = ft_substr(input, *i, var_len); // Extract the variable name from the input string and store it in var_name
	if (!var_name)
		return (NULL);
	var_value = getenv_from_envp(shell, var_name); // Get the value of the variable from the environment variables
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


// void expand_variables_in_tokens(t_shell_data *shell)
// {
//     t_command *cmd;
//     t_token *token;
//     char *expanded_value;
//     int var_not_found_flag;

//     cmd = shell->commands;
//     while (cmd)
//     {
//         token = cmd->token_list;
//         while (token)
//         {
//             if (!token->is_operator)
//             {
//                 var_not_found_flag = 0;

//                 // Store the original value before expansion
//                 token->original_value = ft_strdup(token->value);
//                 if (!token->original_value)
//                 {
//                     ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
//                     g_data.exit_status = 1;
//                     // Handle the error appropriately (e.g., set an error flag, clean up, etc.)
//                     return;
//                 }

//                 // Expand the variable
//                 expanded_value = expand_variables_in_token(token->value, &var_not_found_flag);
//                 if (!expanded_value)
//                 {
//                     ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
//                     free(token->original_value);
//                     token->original_value = NULL;
//                     g_data.exit_status = 1;
//                     // Handle the error appropriately
//                     return;
//                 }

//                 // Replace the token value with the expanded value
//                 free(token->value);
//                 token->value = expanded_value;
//                 token->var_not_found = var_not_found_flag;
//             }
//             token = token->next;
//         }
//         cmd = cmd->next;
//     }
// }

