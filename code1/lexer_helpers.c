#include "minishell.h"

// Handle operator tokens
int	handle_operator(char *input, int i, t_command *cmd)
{
	int		start;
	char	*op;

	start = i;
	if (input[i] == '|' || input[i] == '<' || input[i] == '>')
	{
		if (input[i + 1] == input[i])
			i += 2;
		else
			i++;
		op = ft_substr(input, start, i - start);
		add_token_to_list(&cmd->token_list, op, &cmd->index, 1);
	}
	return (i);
}

// Handle word tokens, including quoted strings and variable expansion
int	handle_word(char *input, int i, t_shell *shell, t_command *cmd)
{
	int		start;
	char	*word;

	start = i;
	while (input[i] && !ft_isspace(input[i]) && !is_operator_char(input[i]))
	{
		if (input[i] == '\'' || input[i] == '\"')
			i = skip_quotes(input, i);
		else
			i++;
	}
	word = ft_substr(input, start, i - start);
	word = expand_variables(word, shell, &cmd->var_not_found_flag);
	add_token_to_list(&cmd->token_list, word, &cmd->index, 0);
	return (i);
}

// Skip over quoted strings
int	skip_quotes(char *input, int i)
{
	char	quote;

	quote = input[i++];
	while (input[i] && input[i] != quote)
		i++;
	if (input[i] == quote)
		i++;
	return (i);
}
