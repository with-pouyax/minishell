#include "../minishell.h"

int	validate_tokens_in_command(t_shell_data *shell, t_command *cmd)
{
	t_token		*token;

	token = cmd->token_list;
	while (token)
	{
		if (token->is_operator)
		{
			if (!is_valid_operator(token->value))
			{
				handle_syntax_error(shell, token);
				return (1);
			}
		}
		token = token->next;
	}
	return (0);
}

int	validate_operators(t_shell_data *shell)
{
	t_command	*cmd;
	int			error_found;

	error_found = 0;
	cmd = shell->commands;
	while (cmd && !error_found)
	{
		error_found = validate_tokens_in_command(shell, cmd);
		cmd = cmd->next;
	}
	if (error_found)
	{
		free_commands(shell);
		shell->commands = NULL;
	}
	return (error_found);
}

int	add_char_to_token(char **token, char c)
{
	char	tmp[2];
	char	*new_token;

	tmp[0] = c;
	tmp[1] = '\0';
	(void)tmp;
	new_token = ft_strjoin_safe(*token, tmp);
	if (!new_token)
	{
		free(*token);
		*token = NULL;
		return (1);
	}
	free(*token);
	*token = new_token;
	return (0);
}
/*****************************************************************************/
// 🎯 Purpose  :  to skip the quoted word
/*****************************************************************************/
//
// 🔹 Parameters:                                                             
//     🏷  input -> user input
//     🏷  i -> the index of the first non-space character in the input                              
//
// 🔄 Returns   :  index
//
/*****************************************************************************/
// 💡 Notes:                                                                  
//     1- we store the current character in the quote variable and increment
//        the index.
//     2- using a while loop we iterate over the input characters as long as
//        the current character is not the same as the quote we already stored
//        in the quote variable.
//     3- if the first character after the loop stop is the same as the quote
//        we increment the index and return the index.
/******************************************************************************/
int	skip_quotes(char *input, int i)
{
	char	quote;

	quote = input[i++];
	while (input[i] && input[i] != quote)
		i++;
	if (input[i] == quote)
		i++;
	return (i);
}
