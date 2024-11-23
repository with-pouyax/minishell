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

int handle_redirection(char *op, char *input, int *i, t_command *cmd, int *redir_count)
{
    t_redirection *new_redir;
    char          *filename;

    filename = NULL;

    new_redir = malloc(sizeof(t_redirection)); // Allocate memory for the new redirection 
    if (!new_redir)
        return (1);
    ft_bzero(new_redir, sizeof(t_redirection)); // Zero out the memory

    // Set redirection type
    if (!ft_strcmp(op, "<")) // If the operator is '<'
        new_redir->type = REDIR_INPUT; // Set the redirection type to REDIR_INPUT
    else if (!ft_strcmp(op, ">"))
        new_redir->type = REDIR_OUTPUT;
    else if (!ft_strcmp(op, ">>"))
        new_redir->type = REDIR_APPEND;
    else if (!ft_strcmp(op, "<<"))
        new_redir->type = REDIR_HEREDOC;

    new_redir->redir_number = (*redir_count)++;


    // Skip spaces
    while (input[*i] && ft_isspace(input[*i]))
        (*i)++;

    // Collect the filename or delimiter
    if (collect_word(input, i, &filename)) // Collect the filename or delimiter
    {
        free(new_redir);
        return (1);
    }
    if (new_redir->type == REDIR_HEREDOC) // If the redirection type is REDIR_HEREDOC
        new_redir->delimiter = filename; // Set the delimiter
    else
        new_redir->filename = filename;

    // Add to the redirections list
    add_redirection(&(cmd->redirections), new_redir); // Add the redirection to the redirections list 
    return (0);
}

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



	//printf("add_token: Added token #%d: %s (Operator: %d)\n", new_token->index, new_token->value, is_operator); // ##debug print

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

	//printf("initialize_new_token: Initialized token #%d: %s\n", new_token->index, new_token->value); // ##debug print
}

