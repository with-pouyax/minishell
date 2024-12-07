#include "../../minishell.h"

int tokenize_command(t_shell_data *shell, t_command *cmd)
{
    int i;

    i = 0;
    while (cmd->command_string[i])                                           //we loop through the characters of the command
    {
        skip_cmd_spaces(cmd->command_string, &i);                           
        if (cmd->command_string[i])                                          //if there is there is a character there
        {
            if (is_operator_char(cmd->command_string[i]))                    //if the character is an operator('|', '<', '>')
            {
                if (process_operator(shell, &i, cmd))						 // we process the operator and store it in the command struct
                    return (tokenize_command_error(cmd));
            }
            else                                                             // if the character is not an operator
            {
                if (process_word(shell, cmd->command_string, &i, cmd))		// we process the word and store it in the command struct
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

    start = *i; 															  //we store the index of the character we recieved as start
    if (is_operator_char(cmd->command_string[*i]))                            //if the character is an operator
    {
        (*i)++;                                        						  // we go to the next character
        if ((cmd->command_string[*i - 1] == '<' || cmd->command_string[*i - 1] == '>') &&
            cmd->command_string[*i] == cmd->command_string[*i - 1])           //if the lst character is '<' or '>' and current character is equal to the last character
            (*i)++;                                                           //we go to the next character
    }
    op = ft_substr(cmd->command_string, start, *i - start);                   //we store the operator we found in op using i as the end index and start as the start index
    return op;
}



//==================================================

int	handle_redirection_operator(t_shell_data *shell, char *op, t_command *cmd, int *i)
{
	cmd->current_op = op;                                          // first we store the operator in the our struct
	if (handle_redirection(shell, cmd->command_string, i, cmd))    // then we handle the redirection operator
		return (1);
	return (0);
}

int	handle_pipe_operator(char *op, t_command *cmd)
{
	(void)cmd;
    // The pipe operator is a separator and should not be added as a token within a command.
    free(op);
    return (0);
}


int	handle_invalid_operator(t_shell_data *shell, char *op)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
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

	op = collect_operator(cmd, i);                                     // first we store the operator in op                
	if (!op)
		return (1);
	if (is_valid_operator(op))                                         // if the operator we stored is a valid operator
	{
		if (is_redirection_operator(op))							   // if the operator is a redirection operator
			ret = handle_redirection_operator(shell, op, cmd, i);      // we handle the redirection operator 
		else                                                           // if it is a pipe operator
			ret = handle_pipe_operator(op, cmd);                       // we handle the pipe operator
		if (ret)													   
			return (1);
	}
	else  														       // if the operator is not a valid operator
	{
		ret = handle_invalid_operator(shell, op);                      // we handle the invalid operator
		return (ret);
	}
	return (0);
}

//==================================================


