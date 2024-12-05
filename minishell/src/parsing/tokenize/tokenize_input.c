#include "../../minishell.h"

int tokenize_command(t_shell_data *shell, t_command *cmd)
{
    int i;

    i = 0;
    while (cmd->command_string[i])
    {
        skip_cmd_spaces(cmd->command_string, &i);
        if (cmd->command_string[i])
        {
            if (is_operator_char(cmd->command_string[i]))
            {
                if (process_operator(shell, &i, cmd))
                    return (tokenize_command_error(cmd));
            }
            else
            {
                if (process_word(shell, cmd->command_string, &i, cmd))
                    return (tokenize_command_error(cmd));
            }
        }
    }
    return (0);
}

void	skip_cmd_spaces(char *str, int *i)
{
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
}

int	process_token(t_shell_data *shell, t_command *cmd, int *i)
{
	int	ret;

	
	if (is_operator_char(cmd->command_string[*i])) // if the character is an operator
		ret = process_operator(shell, i, cmd); // process the operator
	else // if the character is not an operator
		ret = process_word(shell, cmd->command_string, i, cmd); // process the word
	return (ret);
}
int	handle_initial_pipe(char *input, int *i)
{
	if (input[*i] == '|' && (input[*i + 1] == '<' || input[*i + 1] == '>') && !ft_isspace(input[*i + 1]))
	{
		(*i)++;
	}
	return (0);
}

char *collect_operator(t_command *cmd, int *i)
{
    char *op;
    int start;

    start = *i;
    if (is_operator_char(cmd->command_string[*i]))
    {
        (*i)++;
        // Check for double-character operators like '<<' or '>>'
        if ((cmd->command_string[*i - 1] == '<' || cmd->command_string[*i - 1] == '>') &&
            cmd->command_string[*i] == cmd->command_string[*i - 1])
            (*i)++;
    }
    op = ft_substr(cmd->command_string, start, *i - start);
    return op;
}


int	process_collected_operator(t_shell_data *shell, char *op, t_command *cmd, int *i)
{
	int	ret;

	if (is_redirection_operator(op)) // if the operator is redirection operator
	{
		cmd->current_op = op; // set the current operator to the collected operator
		
		if (handle_redirection(shell, shell->input, i, cmd)) // handle redirection
			return (1);
		return (0);
	}
	ret = add_token(op, &cmd->token_list, &cmd->token_index, 1);
	if (ret)
	{
		free(op);
		return (1);
	}
	
	process_operator_details(op, cmd, i, &cmd->token_index);
	return (0);
}


int process_operator(t_shell_data *shell, int *i, t_command *cmd)
{
    char *op;

    op = collect_operator(cmd, i);
    if (!op)
        return 1;
    if (is_valid_operator(op))
    {
        if (is_redirection_operator(op))
        {
            cmd->current_op = op; // Store op in t_command
            if (handle_redirection(shell, shell->input, i, cmd))
                return 1;
        }
        else // It's a pipe '|'
        {
            if (add_token(op, &cmd->token_list, &cmd->token_index, 1))
            {
                free(op);
                return 1;
            }
        }
    }
    else // Invalid operator
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
        ft_putstr_fd(op, STDERR_FILENO);
        ft_putstr_fd("'\n", STDERR_FILENO);
        shell->exit_status = 2;
        free(op);
        return 1;
    }
    return 0;
}

void	process_operator_details(char *op, t_command *cmd, int *i, int *token_index)
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
		(*token_index)++;
}
