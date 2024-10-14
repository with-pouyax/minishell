#include "minishell.h"

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->heredoc = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	add_command(t_command **head, t_command *new_cmd)
{
	t_command	*current;

	if (!*head)
	{
		*head = new_cmd;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

int	handle_redirection(t_token **tokens, t_command *cmd)
{
	t_token	*token;
	int		fd;

	token = *tokens;
	if (!token->next || token->next->type != WORD)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token", STDERR_FILENO);
		return (1);
	}
	token = token->next;
	if ((*tokens)->type == INPUT_REDIRECT)
	{
		fd = open(token->value, O_RDONLY);
		if (fd < 0)
			return (print_error(token->value));
		if (cmd->input_fd != STDIN_FILENO)
			close(cmd->input_fd);
		cmd->input_fd = fd;
	}
	else if ((*tokens)->type == OUTPUT_REDIRECT)
	{
		fd = open(token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (print_error(token->value));
		if (cmd->output_fd != STDOUT_FILENO)
			close(cmd->output_fd);
		cmd->output_fd = fd;
	}
	else if ((*tokens)->type == APPEND_REDIRECT)
	{
		fd = open(token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			return (print_error(token->value));
		if (cmd->output_fd != STDOUT_FILENO)
			close(cmd->output_fd);
		cmd->output_fd = fd;
	}
	else if ((*tokens)->type == HEREDOC)
	{
		if (handle_heredoc(token->value, cmd) != 0)
			return (1);
	}
	*tokens = token->next;
	return (0);
}

int	parse_input(char *input)
{
	t_token		*tokens;
	t_command	*cmd_list;
	int			ret;

	tokens = NULL;
	ret = tokenize_input(input, &tokens);
	if (ret != 0)
	{
		free_tokens(tokens);
		return (1);
	}
	cmd_list = parse_tokens(tokens);
	free_tokens(tokens);
	if (!cmd_list)
		return (1);
	g_cmd_list = cmd_list;
	return (0);
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*cmd_list;
	t_command	*cmd;
	int			arg_count;

	cmd_list = NULL;
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `|'", STDERR_FILENO);
			return (NULL);
		}
		cmd = create_command();
		arg_count = 0;
		while (tokens && tokens->type != PIPE)
		{
			if (tokens->type == WORD)
			{
				cmd->args = realloc(cmd->args, sizeof(char *) * (arg_count + 2));
				cmd->args[arg_count++] = ft_strdup(tokens->value);
				cmd->args[arg_count] = NULL;
				if (!cmd->cmd)
					cmd->cmd = ft_strdup(tokens->value);
			}
			else if (tokens->type == INPUT_REDIRECT || tokens->type == OUTPUT_REDIRECT
				|| tokens->type == APPEND_REDIRECT || tokens->type == HEREDOC)
			{
				if (handle_redirection(&tokens, cmd) != 0)
				{
					free_command(cmd);
					return (NULL);
				}
				continue ;
			}
			else
			{
				ft_putendl_fd("minishell: syntax error", STDERR_FILENO);
				free_command(cmd);
				return (NULL);
			}
			tokens = tokens->next;
		}
		add_command(&cmd_list, cmd);
		if (tokens)
			tokens = tokens->next;
	}
	return (cmd_list);
}

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->value);
		free(temp);
	}
}

void	free_command(t_command *cmd)
{
	if (cmd->cmd)
		free(cmd->cmd);
	if (cmd->args)
		free_array(cmd->args);
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
	free(cmd);
}

int	print_error(char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(message);
	return (1);
}
