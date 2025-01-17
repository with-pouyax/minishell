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

int	handle_initial_pipe(char *input, int *i)
{
	if (input[*i] == '|' && (input[*i + 1] == '<' || input[*i + 1] == '>') && \
	!ft_isspace(input[*i + 1]))
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
	(void)start;
    if (is_operator_char(cmd->command_string[*i]))
    {
        (*i)++;
        if ((cmd->command_string[*i - 1] == '<' || cmd->command_string[*i - 1] == '>') &&
            cmd->command_string[*i] == cmd->command_string[*i - 1])
            (*i)++;
    }
    op = ft_substr(cmd->command_string, start, *i - start);
    if (!op)
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
	return op;
}


int	handle_redirection_operator(t_shell_data *shell, char *op, \
t_command *cmd, int *i)
{
	cmd->current_op = op;
	if (handle_redirection(shell, cmd->command_string, i, cmd))
		return (1);
	return (0);
}

int	handle_pipe_operator(char *op, t_command *cmd)
{
	(void)cmd;
    free(op);
    return (0);
}


int	handle_invalid_operator(t_shell_data *shell, char *op)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", \
	STDERR_FILENO);
	ft_putstr_fd(op, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	shell->exit_status = 2;
	free(op);
	return (1);
}

int	process_operator(t_shell_data *shell, int *i, t_command *cmd)
{
	char	*op;
	int		ret;

	op = collect_operator(cmd, i);               
	if (!op)
		return (1);
	if (is_valid_operator(op))
	{
		if (is_redirection_operator(op))
			ret = handle_redirection_operator(shell, op, cmd, i);
		else
			ret = handle_pipe_operator(op, cmd);
		if (ret)													   
			return (1);
	}
	else 
	{
		ret = handle_invalid_operator(shell, op);
		return (ret);
	}
	return (0);
}
