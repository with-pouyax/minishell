/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:05:39 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 22:21:45 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  redirections ->
//     🏷  new_redir ->
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- if there is no redirection we put the new redirection into the
//        redirections.
//     2- if there is a redirection we loop over the redirections and put the
//        new redirection at the end.
//     3- we set the redirection number to the current redirection number + 1.
//
/******************************************************************************/

void	add_redirection(t_redirection **redirections, t_redirection *new_redir)
{
	t_redirection	*current;

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
/*****************************************************************************/
//                  No explanation needed for this function
/*****************************************************************************/

int	starts_with_operator_char(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  new_redir -> the redirection we want to prepare
//     🏷  filename_or_delimiter
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- we put the delimiter into the redirection structure.
//     2-  we call read_heredoc_content() to read the heredoc content.
//         a- if there is an error we free the delimiter and the redirection
//            structure and return 1.
//     3- at the end we return 0.
//
/******************************************************************************/

int	handle_heredoc_redirection(t_shell_data *shell, t_redirection *new_redir, \
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

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  word ->
//
// 🔄 Returns   : word
//
/*****************************************************************************/
// 💡 Notes:
//     1- we loop over the word and whenever we find a quote we remove it by
//        shifting the characters to the left.
//     2- at the end we return the word.
//
/******************************************************************************/

char	*rm_quotes(char *word)
{
	int	i;
	int	j;

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
	return (word);
}

/*****************************************************************************/
// 🎯 Purpose  :  initialize memory for word and collect the word from the
//                input and store it in word.
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  ctx -> our parse context (shell, input, i, redirection linked list)
//     🏷  word -> the word we collected and expanded
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- we set the word to NULL to avoid any garbage value.
//     2- using collect_word() we collect the word from the input and store it
//        in word.
//        a- if there is an error we return 1.
//     3- if there is no word or the word is empty we free the word
//        and return 1
//     4- if everything is fine we return 0.
/******************************************************************************/

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

/*****************************************************************************/
// 🎯 Purpose  :  collec and expand redirection word
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  words -> our expanded words structure
//     🏷  word  -> word is the filename or delimiter
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using save_and_expand_word() we save and expand the word and store it
//        in words.expanded and words.original.
//        a- if there is an error we free the word and return 1.
//     2- at the end we return 0.
//
/******************************************************************************/

int	handle_non_heredoc(t_shell_data *shell, char *word, t_expanded_words *words)
{
	if (save_and_expand_word(shell, word, &(words->expanded), \
&(words->original)))
	{
		free(word);
		return (1);
	}
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  word  -> word is the filename or delimiter
//     🏷  words -> our expanded words structure
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using rm_quotes() we remove the quotes from the word.
//     2- we store the word in words.expanded and words.original.
//     3- if there is an error duplicating the word in words.expanded or
//        words.original we free the word
//        a- if there is a word in words.expanded we free it.
//        b- if there is a word in words.original we free it.
//        c- we set the error flag to 4.
//        d- we free the word and return 1.
//     4- at the end we return 0.
//
/******************************************************************************/

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

/*****************************************************************************/
// 🎯 Purpose  :  collec and expand redirection word
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  ctx -> our parse context (shell, input, i, redirection linked list)
//     🏷  words -> our expanded words structure
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using init_and_collect_word() we collect and expand the redirection
//        word and store it in word.
//        a- if there is an error we return 1.
//     2- if the redirection type is not heredoc we call handle_non_heredoc()
//        we call handle_non_heredoc() and store the return value of it
//        in error
//        a- if there is an error we return 1.
//     3- if the redirection type is heredoc we call process_heredoc_word()
//        and store the return value of it in error.
//        a- if there is an error we return 1.
//     4- at the end we free the word and return 0.
//
/******************************************************************************/

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

/*****************************************************************************/
//      here we assign the redirection word to the redirection linked list
/*****************************************************************************/

void	assign_redirection(t_shell_data *shell, char *expanded_word, \
char *original_word)
{
	shell->filename_or_delimiter = expanded_word;
	free(original_word);
}

/*****************************************************************************/
// 🎯 Purpose  :  process the filename or delimiter in the redirection
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  expanded_word
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using is_valid_operator() we check if the expanded word is a valid
//        operator.
//        a- if it is return 1 it means it is an invalid operator, so if it is
//           an invalid operator or starts with an operator.
//           I- we print an error message.
//           II- we set the exit status to 2.
//           III- we return 1.
//     2- if it is a valid operator and does not start with an operator we
//        return 0.
//
//
/******************************************************************************/

int validate_expanded_word(t_shell_data *shell, char *expanded_word)
{
	if (is_valid_operator(expanded_word) || \
	starts_with_operator_char(expanded_word[0]))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", \
		STDERR_FILENO);
		ft_putstr_fd(expanded_word, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :  process the filename or delimiter in the redirection
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  input -> user input
//     🏷  i -> index of the current character in the input
//     🏷  redir -> our redirection linked list
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     - ctx stands for context and it is a structure that holds the shell
//	   structure, the input, the index of the current character in the input
//	   and the redirection linked list. we use it to pass less parameters to
//	   the functions.
//     1- using collect_and_expand_redirection_word() we collect and expand the
//        redirection word, redirection word is the filename or the delimiter.
//        a- if there is no redirection word or the redirection word is empty
//           we free words.expanded and words.original and return 1.
//     2- using validate_expanded_word() we validate the expanded word, it will
//        be invalid if it is an operator or starts with an operator.
//        a- if it is invalid we free words.expanded and words.original and
//           return 1.
//     3- using assign_redirection() we assign the redirection word to the
//        redirection linked list.
//     4- now that everything is fine we return 0.
//
/******************************************************************************/

int	process_filename_or_delimiter(t_shell_data *shell, char *input, \
int *i, t_redirection *redir)
{
	t_expanded_words	words;
	t_parse_context		ctx;

	ctx.shell = shell;
	ctx.input = input;
	ctx.i = i;
	ctx.redir = redir;
	if (collect_and_expand_redirection_word(shell, &ctx, &words))
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

int	handle_unexpected_token_error(t_shell_data *shell, t_redirection *new_redir, \
char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", \
	STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	free(new_redir);
	shell->exit_status = 2;
	return (1);
}

/*****************************************************************************/
// 🎯 Purpose  :  process the filename or delimiter in the redirection
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  new_redir  ->
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//
//     1- if the shell error flag is 4 we print an Error
//     2- if the shell error flag is not 4 we print a syntax error
//     3- we free the new redirection
//     4- we set the exit status to 2
//     5- we return 1
//
/******************************************************************************/

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
/*****************************************************************************/
// 💡 Notes:
//     1- no explanation needed.
//
/******************************************************************************/
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

/*****************************************************************************/
// 💡 Notes:
//     1- no explanation needed.
//
/******************************************************************************/

int	handle_syntax_error_s(t_shell_data *shell, t_redirection *new_redir, \
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
/*****************************************************************************/
// 🎯 Purpose  :  skip_white spaces
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  input
//     🏷  i
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- using a while loop we iterate over the input characters as long as
//        the current character is a white space we increment the index.
//
/******************************************************************************/

void	skip_whitespace(char *input, int *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}

/*****************************************************************************/
// 🎯 Purpose  :  create a new redirection and store it in new_redir linked list
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  op -> our operation
// 🔄 Returns   :  our new redirection
//
/*****************************************************************************/
// 💡 Notes:
//     1- we allocate memory for our new redirection.
//        a- if there is an error we print an error message and return NULL.
//     2- using ft_bzero() we set the new redirection elements to 0.
//     3- using ft_strcmp() we check the operation and set the type of the
//        propper redirection.
//     4- we return the new redirection.
//
/******************************************************************************/

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

/*****************************************************************************/
// 🎯 Purpose  :  handle pipe errors
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  new_redir -> the redirection we want to prepare
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- if type of new_redir is REDIR_HEREDOC we call
//     handle_heredoc_redirection()
//        a- if there is an error we return 1.
//     2- if the type of new_redir is not REDIR_HEREDOC we set the filename
//        to the shell filename_or_delimiter.
//
/******************************************************************************/

int	finalize_redirection(t_shell_data *shell, t_redirection *new_redir)
{
	if (new_redir->type == REDIR_HEREDOC)
	{
		if (handle_heredoc_redirection(shell, new_redir, \
		shell->filename_or_delimiter))
			return (1);
	}
	else
		new_redir->filename = shell->filename_or_delimiter;
	shell->filename_or_delimiter = NULL;
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :  handle pipe errors
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  input -> user input
//     🏷  i -> index of the current character in the input
//     🏷  new_redir -> the redirection we want to prepare
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the next character in the input is |, >, <, we call
//        handle_syntax_error_s() to handle the error.
//     2- if none of the above,  still there is an error and we call
//        handle_syntax_error_s() to handle the error.
//
/******************************************************************************/

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

/*****************************************************************************/
// 💡 Notes:
//     1- no explanation needed.
//
/******************************************************************************/

int	handle_greater_operator(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir)
{
	if (input[*i + 1] == '>')
		return (handle_syntax_error_s(shell, new_redir, ">>"));
	else
		return (handle_syntax_error_s(shell, new_redir, ">"));
}
/*****************************************************************************/
// 💡 Notes:
//     1- no explanation needed.
//
/******************************************************************************/

int	handle_less_operator(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir)
{
	if (input[*i + 1] == '<')
		return (handle_syntax_error_s(shell, new_redir, "<<"));
	else
		return (handle_syntax_error_s(shell, new_redir, "<"));
}

/*****************************************************************************/
// 🎯 Purpose  :  check operators errors
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  input -> user input
//     🏷  i -> index of the current character in the input
//     🏷  new_redir -> the redirection we want to prepare
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the current character in the input is | we call handle_pipe_op()
//        to handle the pipe operator error.
//     2- if the current character in the input is > we call
//     handle_greater_operator() to handle the greater operator error.
//     3- if the current character in the input is < we call
//     handle_less_operator() to handle the less operator error.
//     4- if none of the above,  means there is no operator error and we
//     return 0.
//
/******************************************************************************/

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

/*****************************************************************************/
// 🎯 Purpose  :  prepare the redirection and store it in new_redir linked list
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  cmd -> our command linked list
//     🏷  new_redir -> the redirection we want to prepare
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using create_new_redirection() we create a new redirection and store
//        it in new_redir linked list.
//     2- we free op because we have already stored it in new_redir.
//     3- we set the current operator to NULL because we have already stored it
//        in new_redir.
//     4- if we successfully created the new redirection we return 0, else 1.
//
/******************************************************************************/

int	prepare_redirection(t_command *cmd, t_redirection **new_redir)
{
	char	*op;

	op = cmd->current_op;
	if (!op)
		return (1);
	*new_redir = create_new_redirection(op);
	free(op);
	cmd->current_op = NULL;
	return (!(*new_redir));
}

/*****************************************************************************/
// 🎯 Purpose  :  handle redirection
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  input -> user input
//     🏷  i -> index of the current character in the input
//     🏷  cmd -> our command linked list
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- we set the filename_or_delimiter to NULL.
//     2- using prepare_redirection() we prepare the redirection.
//        a- if there is an error we return 1.
//     3- using skip_whitespace() we skip the whitespaces in the input.
//     4- using check_operator_error() we check if there is an operator error.
//        a- if there is an error we return 1.
//     5- using process_filename_or_delimiter() we process the filename
//        or delimiter.
//        a- if there is an error we return 1.
//     6- using finalize_redirection() we finalize the redirection [???]
//        a- if there is an error we return 1.
//     7- using add_redirection() we add this new_redir to the redirections
//        linked list.
//     8- if everything is fine we return 0.
//
/******************************************************************************/

int handle_redirection(t_shell_data *shell, char *input, int *i, \
t_command *cmd)
{
	t_redirection	*new_redir;

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
/*****************************************************************************/
// 🎯 Purpose  :  check if op is a redirection
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  op -> the character we recieved as operator.
//
// 🔄 Returns   :  1 if op is redirection, else 0
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the op is <, >, >>, or << we return 1 else 0.
//
/******************************************************************************/
int	is_redirection_operator(char *op)
{
	return (!ft_strcmp(op, "<") || !ft_strcmp(op, ">") || \
	!ft_strcmp(op, ">>") || !ft_strcmp(op, "<<"));
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  token_value ->
//     🏷  token_list ->
//     🏷  index ->
//     🏷  is_operator ->
//
// 🔄 Returns   :  1 if op is redirection, else 0
//
/*****************************************************************************/
// 💡 Notes:
//     1- we allocate memory for a new token.
//        a- if there is an error we print an error message and return 1.
//     2- using initialize_new_token() we initialize the new token.
//     3- if there is no token_list we put the new token in the token_list.
//     4- if there is a token_list we loop over the token_list and put the new
//        token at the end.
//     5- at the end we return 0.
//
/******************************************************************************/

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

void	initialize_new_token(t_token *new_token, char *token_value, \
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
