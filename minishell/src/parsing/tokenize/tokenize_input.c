#include "../../minishell.h"

int tokenize_command(t_shell_data *shell,t_command *cmd)
{
    int i;
    int index;
    int redir_count; // Initialize redirection count

    i = 0;
    index = 0;
    redir_count = 0;
    while (cmd->command_string[i])
    {
        skip_cmd_spaces(cmd->command_string, &i);
        if (cmd->command_string[i])
        {
            if (process_token(shell, cmd, &i, &index, &redir_count))
                return (tokenize_command_error(cmd));
        }
    }
    return (0);
}



void	skip_cmd_spaces(char *str, int *i)
{
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
}

int process_token(t_shell_data *shell, t_command *cmd, int *i, int *index, int *redir_count)
{
    int ret;

    if (is_operator_char(cmd->command_string[*i]))
        ret = process_operator(shell, cmd->command_string, i, cmd, index, redir_count);
    else
        ret = process_word(shell, cmd->command_string, i, cmd, index);
    return (ret);
}

int process_operator(t_shell_data *shell ,char *input, int *i, t_command *cmd, int *index, int *redir_count)
{
	char	*op;
	int		ret;

	op = ft_strdup("");
	if (!op)
		return (1);
	while (input[*i] && is_operator_char(input[*i])) // Loop through the input string until we find a non-operator character 
	{
		ret = add_char_to_token(&op, input[*i]); // Add the character to the operator string (op)
		if (ret)
			return (free_and_return(op));
		(*i)++;
	}

	if (is_redirection_operator(op)) //if there is redirection operator
    {
        if (handle_redirection(shell, op, input, i, cmd, redir_count))
        {
            free(op);
            return (1);
        }
        free(op);
        return (0);
    }

	ret = add_token(op, &cmd->token_list, index, 1); // Add the operator to the token list
	if (ret)
		return (free_and_return(op));
	process_operator_details(op, cmd, i, index);
	return (0);
}

void	process_operator_details(char *op, t_command *cmd, int *i, int *index)
{
	t_token	*last_token;

	if (!is_valid_operator(op))
		cmd->token_list->wrong_operator = 1;
	last_token = cmd->token_list;
	while (last_token->next)
		last_token = last_token->next;
	if (ft_strcmp(op, "<<") == 0)
	{
		last_token->is_heredoc = 1;
		if (get_heredoc_delimiter(cmd->command_string, i, last_token))
			cmd->token_list->wrong_operator = 1;
	}
	else
		(*index)++;
}











//this function not used

int	process_quoted_word(char *input, int *i, char **word)
{
	char	quote;
	int		ret;

	quote = input[*i];
	(*i)++; // Skip the opening quote
	while (input[*i] && input[*i] != quote)
	{
		ret = add_char_to_token(word, input[*i]);
		(*i)++;
		if (ret)
			return (1);
	}
	if (input[*i] == quote)
	{
		(*i)++; // Skip the closing quote
	}
	else
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}


