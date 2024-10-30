#include "minishell.h"

t_command	*create_command(char *cmd_str, int index)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->command_string = cmd_str;
	cmd->index = index;
	cmd->token_list = NULL;
	cmd->next = NULL;
	return (cmd);
}
void	skip_cmd_spaces(char *str, int *i)
{
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
}

int	process_token(t_command *cmd, int *i, int *index)
{
	int	ret;

	if (is_operator_char(cmd->command_string[*i]))
		ret = process_operator(cmd->command_string, i, cmd, index);
	else
		ret = process_word(cmd->command_string, i, cmd, index);
	return (ret);
}

int	tokenize_command(t_command *cmd)
{
	int	i;
	int	index;

	i = 0;
	index = 0;
	while (cmd->command_string[i])
	{
		skip_cmd_spaces(cmd->command_string, &i);
		if (cmd->command_string[i])
		{
			if (process_token(cmd, &i, &index))
				return (tokenize_command_error(cmd));
		}
	}
	return (0);
}

int	tokenize_command_error(t_command *cmd)
{
	free_tokens(cmd->token_list);
	cmd->token_list = NULL;
	return (1);
}

int	process_operator(char *input, int *i, t_command *cmd, int *index)
{
	char	*op;
	int		ret;

	op = ft_strdup("");
	if (!op)
		return (1);
	while (input[*i] && is_operator_char(input[*i]))
	{
		ret = add_char_to_token(&op, input[*i]);
		if (ret)
			return (free_and_return(op));
		(*i)++;
	}
	ret = add_token(op, &cmd->token_list, index, 1);
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
		if (process_heredoc_delimiter(cmd->command_string, i, last_token))
			cmd->token_list->wrong_operator = 1;
	}
	else
		(*index)++;
}

int	free_and_return(char *str)
{
	free(str);
	return (1);
}

int	collect_word(char *input, int *i, char **word)
{
	int	ret;

	while (input[*i] && !ft_isspace(input[*i]) && !is_operator_char(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '\"')
		{
			ret = process_quoted_word(input, i, word);
			if (ret)
				return (1);
		}
		else
		{
			ret = add_char_to_token(word, input[*i]);
			(*i)++;
			if (ret)
				return (1);
		}
	}
	return (0);
}

int	process_word(char *input, int *i, t_command *cmd, int *index)
{
	char	*word;

	word = ft_strdup("");
	if (!word)
		return (1);
	if (collect_word(input, i, &word))
	{
		free(word);
		return (1);
	}
	if (add_token(word, &cmd->token_list, index, 0))
	{
		free(word);
		return (1);
	}
	return (0);
}

int	process_quoted_word(char *input, int *i, char **word)
{
	char	quote;
	int		ret;

	quote = input[*i];
	ret = add_char_to_token(word, input[*i]);
	(*i)++;
	while (input[*i] && input[*i] != quote)
	{
		ret = add_char_to_token(word, input[*i]);
		(*i)++;
		if (ret)
			return (1);
	}
	if (input[*i] == quote)
	{
		ret = add_char_to_token(word, input[*i]);
		(*i)++;
		if (ret)
			return (1);
	}
	return (0);
}

