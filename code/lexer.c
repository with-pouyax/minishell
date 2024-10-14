#include "minishell.h"

t_token	*create_token(char *value, int type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

char	*get_word(char *input, int *i)
{
	int		start;
	char	*word;

	start = *i;
	while (input[*i] && !is_whitespace(input[*i]) && !is_special_char(input[*i]))
		(*i)++;
	word = ft_substr(input, start, *i - start);
	return (word);
}

char	*get_quoted_word(char *input, int *i)
{
	int		start;
	char	quote;
	char	*word;

	quote = input[*i];
	start = ++(*i);
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] != quote)
	{
		ft_putendl_fd("minishell: syntax error: unclosed quote", STDERR_FILENO);
		return (NULL);
	}
	word = ft_substr(input, start, *i - start);
	(*i)++;
	return (word);
}

int	tokenize_input(char *input, t_token **tokens)
{
	int		i;
	char	*value;
	t_token	*token;

	i = 0;
	while (input[i])
	{
		if (is_whitespace(input[i]))
			i++;
		else if (input[i] == '\'' || input[i] == '\"')
		{
			value = get_quoted_word(input, &i);
			if (!value)
				return (1);
			token = create_token(value, WORD);
			add_token(tokens, token);
		}
		else if (is_special_char(input[i]))
		{
			if (input[i] == '>' && input[i + 1] == '>')
			{
				token = create_token(ft_strdup(">>"), APPEND_REDIRECT);
				i += 2;
			}
			else if (input[i] == '<' && input[i + 1] == '<')
			{
				token = create_token(ft_strdup("<<"), HEREDOC);
				i += 2;
			}
			else if (input[i] == '>')
			{
				token = create_token(ft_strdup(">"), OUTPUT_REDIRECT);
				i++;
			}
			else if (input[i] == '<')
			{
				token = create_token(ft_strdup("<"), INPUT_REDIRECT);
				i++;
			}
			else if (input[i] == '|')
			{
				token = create_token(ft_strdup("|"), PIPE);
				i++;
			}
			add_token(tokens, token);
		}
		else
		{
			value = get_word(input, &i);
			token = create_token(value, WORD);
			add_token(tokens, token);
		}
	}
	return (0);
}
