#include "../../minishell.h"

void add_redirection(t_redirection **redirections, t_redirection *new_redir)
{
    t_redirection *current;

    if (!*redirections)
        *redirections = new_redir;
    else
    {
        current = *redirections;
        while (current->next)
            current = current->next;
        current->next = new_redir;
        new_redir->redir_number = current->redir_number + 1; 
	}
}

int starts_with_operator_char(char c)
{
    return (c == '<' || c == '>' || c == '|');
}


int	handle_heredoc_redirection(t_shell_data *shell, t_redirection *new_redir,
							char *filename_or_delimiter)
{
	new_redir->delimiter = filename_or_delimiter;
	if (read_heredoc_content(shell, new_redir))
	{
		free(new_redir->delimiter);
		free(new_redir);
		return (1);
	}
	return (0);
}
char *rm_quotes(char *word)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (word[i] != '\0')
    {
        if (word[i] != '\'' && word[i] != '\"')
        {
            word[j] = word[i];
            j++;
        }
        i++;
    }
    word[j] = '\0';
    return word;
}


int	init_and_collect_word(t_parse_context *ctx, char **word)
{
	*word = NULL;
	if (collect_word(ctx->input, ctx->i, word, ctx->shell))
		return (1);
	if (!*word || ft_strlen(*word) == 0)
	{
		free(*word);
		return (1);
	}
	return (0);
}

int	handle_non_heredoc(t_shell_data *shell, char *word, t_expanded_words *words)
{
	if (save_and_expand_word(shell, word, &(words->expanded), &(words->original)))
	{
		free(word);
		return (1);
	}
	return (0);
}

int	process_heredoc_word(t_shell_data *shell, char *word, \
t_expanded_words *words)
{
	rm_quotes(word);
	words->expanded = ft_strdup(word);
	words->original = ft_strdup(word);
	if (!words->expanded || !words->original)
	{
		if (words->expanded)
			free(words->expanded);
		if (words->original)
			free(words->original);
		shell->error_flag = 4;
		free(word);
		return (1);
	}
	return (0);
}

int	collect_and_expand_redirection_word(t_shell_data *shell, \
		t_parse_context *ctx, t_expanded_words *words)
{
	char	*word;
	int		error;

	error = init_and_collect_word(ctx, &word);
	if (error)
		return (1);
	if (ctx->redir->type != REDIR_HEREDOC)
	{
		error = handle_non_heredoc(shell, word, words);
		if (error)
			return (1);
	}
	else
	{
		error = process_heredoc_word(shell, word, words);
		if (error)
			return (1);
	}
	free(word);
	return (0);
}



void	assign_redirection(t_shell_data *shell, char *expanded_word, \
char *original_word)
{
	shell->filename_or_delimiter = expanded_word;
	free(original_word);
}

int	validate_expanded_word(t_shell_data *shell, char *expanded_word)
{
	if (is_valid_operator(expanded_word) ||
		starts_with_operator_char(expanded_word[0]))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `",
					STDERR_FILENO);
		ft_putstr_fd(expanded_word, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}


int process_filename_or_delimiter(t_shell_data *shell, char *input,
                                  int *i, t_redirection *redir)
{
    t_expanded_words    words;
    t_parse_context     ctx;

    ctx.shell = shell;
    ctx.input = input;
    ctx.i = i;
    ctx.redir = redir;
    if (collect_and_expand_redirection_word(shell, &ctx, &words)) //[x]
        return (handle_missing_filename_error(shell, redir));
    if (!words.expanded || ft_strlen(words.expanded) == 0)
    {
        free(words.expanded);
        free(words.original);
        return (handle_missing_filename_error(shell, redir));
    }
    if (validate_expanded_word(shell, words.expanded))
    {
        free(words.expanded);
        free(words.original);
        free_redirections(redir);
        return (1);
    }
    assign_redirection(shell, words.expanded, words.original);
    return (0);
}

int	handle_unexpected_token_error(t_shell_data *shell, t_redirection *new_redir,
								char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	free(new_redir);
	shell->exit_status = 2;
	return (1);
}

int	handle_missing_filename_error(t_shell_data *shell, \
t_redirection *new_redir)
{
    if (shell->error_flag == 4)
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
    else
        ft_putstr_fd("syntax error near unexpected token `newline'\n", \
        STDERR_FILENO);
    free(new_redir);
    shell->exit_status = 2;
    return (1);
}

void	ft_putstr_fd2(const char *s, int fd)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

int handle_syntax_error_s(t_shell_data *shell, t_redirection *new_redir, \
const char *unexpected_token)
{
    ft_putstr_fd("minishell: syntax error near unexpected token `", \
    STDERR_FILENO);
    ft_putstr_fd2(unexpected_token, STDERR_FILENO);
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
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (NULL);
	}
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
    if (new_redir->type == REDIR_HEREDOC)
	{
		if (handle_heredoc_redirection(shell, new_redir, shell->filename_or_delimiter))
			return (1);
	}
	else
		new_redir->filename = shell->filename_or_delimiter;
	shell->filename_or_delimiter = NULL;
	return (0);
}



int	handle_pipe_op(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir)
{
	if (input[*i + 1] == '|')
		return (handle_syntax_error_s(shell, new_redir, "||"));
	else if (input[*i + 1] == '>')
		return (handle_syntax_error_s(shell, new_redir, ">|"));
	else if (input[*i + 1] == '<')
		return (handle_syntax_error_s(shell, new_redir, "<|"));
	else if (input[*i + 1] == '\0' || input[*i + 1] == ' ' || \
    input[*i + 1] == '\n')
		return (handle_syntax_error_s(shell, new_redir, "|"));
	else
		return (handle_syntax_error_s(shell, new_redir, "|"));
}

int	handle_greater_operator(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir)
{
	if (input[*i + 1] == '>')
		return (handle_syntax_error_s(shell, new_redir, ">>"));
	else
		return (handle_syntax_error_s(shell, new_redir, ">"));
}

int	handle_less_operator(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir)
{
	if (input[*i + 1] == '<')
		return (handle_syntax_error_s(shell, new_redir, "<<"));
	else
		return (handle_syntax_error_s(shell, new_redir, "<"));
}

int	check_operator_error(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir)
{
	if (input[*i] == '|')
		return (handle_pipe_op(shell, input, i, new_redir));
	else if (input[*i] == '>')
		return (handle_greater_operator(shell, input, i, new_redir));
	else if (input[*i] == '<')
		return (handle_less_operator(shell, input, i, new_redir));
	return (0);
}

int	prepare_redirection(t_command *cmd, t_redirection **new_redir)
{
    char *op = cmd->current_op;

    if (!op)
        return (1);
    *new_redir = create_new_redirection(op);
    free(op);
    cmd->current_op = NULL;
    return (!(*new_redir));
}


int handle_redirection(t_shell_data *shell, char *input, int *i, \
t_command *cmd)
{
    t_redirection *new_redir;

    shell->filename_or_delimiter = NULL;
    if (prepare_redirection(cmd, &new_redir))
        return (1);
    
    skip_whitespace(input, i);
    if (check_operator_error(shell, input, i, new_redir))
        return (1);
    if (process_filename_or_delimiter(shell, input, i, new_redir))
        return (1);
    if (finalize_redirection(shell, new_redir))
        return (1);
    add_redirection(&(cmd->redirections), new_redir);
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
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (1);
	}
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
	new_token->original_value = NULL;
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
	new_token->is_end = 0;
	new_token->next = NULL;
}
