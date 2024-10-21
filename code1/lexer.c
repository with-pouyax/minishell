/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by yourname          #+#    #+#             */
/*   Updated: 2024/10/21 15:49:56 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static void	add_char_to_token(char **token, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	*token = ft_strjoin_free(*token, ft_strdup(tmp));
}

static void	process_operator(char *input, int *i, t_command *cmd, int *index)
{
	char	*op;

	op = ft_strdup("");
	while (input[*i] && is_operator_char(input[*i]))
	{
		add_char_to_token(&op, input[*i]);
		(*i)++;
	}
	add_token(op, &cmd->token_list, index, 1);
}

static void	process_word(char *input, int *i, t_command *cmd, int *index)
{
	char	*word;
	char	quote;

	word = ft_strdup("");
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator_char(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '\"')
		{
			quote = input[*i];
			(*i)++;
			while (input[*i] && input[*i] != quote)
			{
				add_char_to_token(&word, input[*i]);
				(*i)++;
			}
			if (input[*i] == quote)
				(*i)++;
		}
		else
		{
			add_char_to_token(&word, input[*i]);
			(*i)++;
		}
	}
	add_token(word, &cmd->token_list, index, 0);
}

void	tokenize_input(char *input, t_shell *shell)
{
	int			i;
	t_command	*cmd;
	int			index;

	i = 0;
	index = 0;
	cmd = malloc(sizeof(t_command));
	cmd->command_string = ft_strdup(input);
	cmd->index = 0;
	cmd->next = NULL;
	cmd->token_list = NULL;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (input[i] && is_operator_char(input[i]))
			process_operator(input, &i, cmd, &index);
		else if (input[i])
			process_word(input, &i, cmd, &index);
	}
	shell->commands = cmd;
}
