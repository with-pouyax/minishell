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
	new_token = ft_strjoin_safe(*token, tmp); //[x]
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
