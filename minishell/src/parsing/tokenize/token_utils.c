#include "../../minishell.h"

void add_redirection(t_redirection **redirections, t_redirection *new_redir)
{
    t_redirection *current;

    if (!*redirections) // If the redirections list is empty
        *redirections = new_redir; // Set the redirections list to the new redirection
    else // If the redirections list is not empty
    {
        current = *redirections; // Set the current redirection to the first redirection in the list
        while (current->next) // Loop through the redirections list until we reach the last redirection
            current = current->next; // Move to the next redirection
        current->next = new_redir; // Add the new redirection to the end of the list
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
	new_redir->delimiter = filename_or_delimiter;
	if (read_heredoc_content(shell, new_redir))
	{
		// Free new_redir and its members if necessary
		free(new_redir->delimiter);
		free(new_redir);
		return (1);
	}
	return (0);
}



int	process_filename_or_delimiter(t_shell_data *shell, char *input, int *i,
								t_redirection *new_redir, char **filename_or_delimiter)
{
	if (collect_word(input, i, filename_or_delimiter, shell))
	{
		free(new_redir);
		return (1);
	}
	if (!*filename_or_delimiter || ft_strlen(*filename_or_delimiter) == 0)
	{
		// Free filename_or_delimiter before returning
		free(*filename_or_delimiter);
		return (handle_missing_filename_error(shell, new_redir));
	}
	if (is_valid_operator(*filename_or_delimiter) ||
		starts_with_operator_char((*filename_or_delimiter)[0]))
	{
		// Free filename_or_delimiter before returning
		free(*filename_or_delimiter);
		return (handle_unexpected_token_error(shell, new_redir, *filename_or_delimiter));
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
	static int		i = 0;

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
	new_redir->redir_number = i++;
	return (new_redir);
}

int	finalize_redirection(t_shell_data *shell, t_redirection *new_redir, char *filename_or_delimiter)
{
    if (new_redir->type == REDIR_HEREDOC)
    {
        if (handle_heredoc_redirection(shell, new_redir, filename_or_delimiter))
            return (1);
    }
    else
        new_redir->filename = filename_or_delimiter;
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
    char *op = cmd->current_op;

    if (!op)
        return (1);
    *new_redir = create_new_redirection(op);
    free(op); // Free op after use
    cmd->current_op = NULL; // Reset current_op after handling
    return (!(*new_redir)); // Return 1 on allocation failure
}



int	handle_redirection(t_shell_data *shell, char *input, int *i, t_command *cmd)
{
    t_redirection	*new_redir;
    char			*filename_or_delimiter = NULL;

    if (prepare_redirection(cmd, &new_redir))
        return (1);
    skip_whitespace(input, i);
    if (check_operator_error(shell, input[*i], new_redir))
        return (1);
    if (process_filename_or_delimiter(shell, input, i, new_redir, &filename_or_delimiter))
        return (1);
    if (finalize_redirection(shell, new_redir, filename_or_delimiter))
        return (1);
    add_redirection(&(cmd->redirections), new_redir);
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
