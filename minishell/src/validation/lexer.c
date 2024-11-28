#include "../minishell.h"

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_valid_operator(char *op)
{
    if (!ft_strcmp(op, "|") || !ft_strcmp(op, "<") ||
        !ft_strcmp(op, ">") || !ft_strcmp(op, ">>") ||
        !ft_strcmp(op, "<<") || !ft_strcmp(op, "|<") ||
        !ft_strcmp(op, "|>") || !ft_strcmp(op, "|>>") ||
		!ft_strcmp(op, "|<<"))
        return (1);
    return (0);
}
int	validate_operators(t_shell_data *shell)
{
    t_command *cmd;
    t_token *token;
    int error_found = 0;

    cmd = shell->commands;
    while (cmd && !error_found)
    {
        token = cmd->token_list;
        while (token && !error_found)
        {
            if (token->is_operator)
            {
                if (!is_valid_operator(token->value))
                {
                    ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
                    ft_putstr_fd(token->value, STDERR_FILENO);
                    ft_putstr_fd("'\n", STDERR_FILENO);
                    shell->exit_status = 2;
                    error_found = 1;
                }
            }
            token = token->next;
        }
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
