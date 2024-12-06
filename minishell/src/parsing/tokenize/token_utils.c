#include "../../minishell.h"

void add_redirection(t_redirection **redirections, t_redirection *new_redir)
{
    t_redirection *current;

    if (!*redirections)                            // If the redirections list is empty
        *redirections = new_redir;                 // Set the redirections list to the new redirection
    else                                           // If the redirections list is not empty
    {
        current = *redirections;                   // Set the current redirection to the first redirection in the list
        while (current->next)                      // Loop through the redirections list until we reach the last redirection
            current = current->next;               // Move to the next redirection
        current->next = new_redir;                 // Add the new redirection to the end of the list
        new_redir->redir_number = current->redir_number + 1; 
	}
}

int starts_with_operator_char(char c)
{
    return (c == '<' || c == '>' || c == '|');
}


//=================================

int	handle_heredoc_redirection(t_shell_data *shell, t_redirection *new_redir,
							char *filename_or_delimiter)
{
	new_redir->delimiter = filename_or_delimiter;         					//we store the delimiter in the new redirection struct
	if (read_heredoc_content(shell, new_redir))                             //we read the content of the heredoc redirection
	{
		// Free new_redir and its members if necessary
		free(new_redir->delimiter);
		free(new_redir);
		return (1);
	}
	return (0);
}



int	process_filename_or_delimiter(t_shell_data *shell, char *input, int *i,
									t_redirection *new_redir)
{
	if (collect_word(input, i, &shell->filename_or_delimiter, shell))                   //we collect the filename or delimiter for the redirection and store it in shell->filename_or_delimiter
	{
		free(new_redir);
		return (1);
	}
	if (!shell->filename_or_delimiter || ft_strlen(shell->filename_or_delimiter) == 0) //we check that the filename is not empty
	{
		// Free shell->filename_or_delimiter before returning
		free(shell->filename_or_delimiter);
		shell->filename_or_delimiter = NULL;
		return (handle_missing_filename_error(shell, new_redir));
	}
	if (is_valid_operator(shell->filename_or_delimiter) ||
		starts_with_operator_char(shell->filename_or_delimiter[0]))                    //we check that the filename is not an operator
	{
		// Free shell->filename_or_delimiter before returning
		free(shell->filename_or_delimiter);
		shell->filename_or_delimiter = NULL;
		return (handle_unexpected_token_error(shell, new_redir, shell->filename_or_delimiter));
	}
	return (0);
}



int	handle_unexpected_token_error(t_shell_data *shell, t_redirection *new_redir,
								char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	// Removed: free(token);
	free(new_redir);
	shell->exit_status = 2;
	return (1);
}

int	handle_missing_filename_error(t_shell_data *shell, t_redirection *new_redir)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
	free(new_redir);
	shell->exit_status = 2;
	return (1);
}






int	handle_syntax_error_s(t_shell_data *shell, t_redirection *new_redir,
						char unexpected_char)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
	ft_putchar_fd(unexpected_char, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	free(new_redir);
	shell->exit_status = 2;
	return (1);
}


void	skip_whitespace(char *input, int *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}


t_redirection	*create_new_redirection(char *op)
{
	t_redirection	*new_redir;

	new_redir = malloc(sizeof(t_redirection));
	if (!new_redir)
		return (NULL);
	ft_bzero(new_redir, sizeof(t_redirection));
	if (!ft_strcmp(op, "<"))
		new_redir->type = REDIR_INPUT;
	else if (!ft_strcmp(op, ">"))
		new_redir->type = REDIR_OUTPUT;
	else if (!ft_strcmp(op, ">>"))
		new_redir->type = REDIR_APPEND;
	else if (!ft_strcmp(op, "<<"))
		new_redir->type = REDIR_HEREDOC;
	
	return (new_redir);
}

int	finalize_redirection(t_shell_data *shell, t_redirection *new_redir)
{
    if (new_redir->type == REDIR_HEREDOC)                                                    //if the redirection is a heredoc redirection
	{
		if (handle_heredoc_redirection(shell, new_redir, shell->filename_or_delimiter))      //we handle the heredoc redirection
			return (1);
	}
	else                                                                                     //if the redirection is not a heredoc redirection
		new_redir->filename = shell->filename_or_delimiter;                                  //we store the filename in the new redirection struct
	shell->filename_or_delimiter = NULL;                                                     //we reset the filename_or_delimiter
	return (0);
}


int	check_operator_error(t_shell_data *shell, char unexpected_char, t_redirection *new_redir)
{
    if (unexpected_char && is_operator_char(unexpected_char))
        return (handle_syntax_error_s(shell, new_redir, unexpected_char));
    return (0);
}


int	prepare_redirection(t_command *cmd, t_redirection **new_redir)
{
    char *op = cmd->current_op;                     // we get the current operator from the command struct

    if (!op)
        return (1);
    *new_redir = create_new_redirection(op);        // we create a new redirection struct
    free(op);
    cmd->current_op = NULL;						    // Reset current_op 
    return (!(*new_redir));                         // if we successfully created the redirection struct, we return 0, else 1
}



int	handle_redirection(t_shell_data *shell, char *input, int *i, t_command *cmd)
{
    t_redirection	*new_redir;                       				//first we create a new redirection struct

    shell->filename_or_delimiter = NULL;
    if (prepare_redirection(cmd, &new_redir))         				//we store the operator in the new redirection struct amd set also the type of redirection
        return (1);
    skip_whitespace(input, i);                         				//we skip the whitespaces after the operator in the input string
    if (check_operator_error(shell, input[*i], new_redir))
        return (1);
    if (process_filename_or_delimiter(shell, input, i, new_redir)) //we store the filename or delimiter for the redirection in shell->filename_or_delimiter
        return (1);
    if (finalize_redirection(shell, new_redir))                    // we check if the word we found is a filename or delimiter and we store it in proper field of the redirection struct
        return (1);
    add_redirection(&(cmd->redirections), new_redir);              // we add the redirection to the list of redirections in the command struct
    return (0);
}




//=================================


int is_redirection_operator(char *op)
{
    return (!ft_strcmp(op, "<") || !ft_strcmp(op, ">") ||
            !ft_strcmp(op, ">>") || !ft_strcmp(op, "<<"));
}

int	add_token(char *token_value, t_token **token_list,
		int *index, int is_operator)
{
	t_token	*new_token;
	t_token	*current;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (1);
	initialize_new_token(new_token, token_value, index, is_operator);
	if (!*token_list)
		*token_list = new_token;
	else
	{
		current = *token_list;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
	return (0);
}

void	initialize_new_token(t_token *new_token, char *token_value,
		int *index, int is_operator)
{
	new_token->value = token_value;
	new_token->original_value = NULL; // Initialize original_value to NULL
	new_token->index = (*index)++;
	new_token->is_operator = is_operator;
	new_token->is_command = 0;
	new_token->is_flag = 0;
	new_token->is_int = 0;
	new_token->var_not_found = 0;
	new_token->wrong_operator = 0;
	new_token->is_heredoc = 0;
	new_token->heredoc_delimiter = NULL;
	new_token->heredoc_file = NULL;
	new_token->is_end = 0; // Initialize is_end to 0
	new_token->next = NULL;
}
