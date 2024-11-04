#include "minishell.h"

int	get_heredoc_delimiter(char *input, int *i, t_token *heredoc_token)
{
	char	*delimiter;
	int		start;

	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	if (!input[*i])
		return (syntax_error_newline());
	start = *i;
	if (input[*i] == '\'' || input[*i] == '\"')
	{
		if (process_quoted_delimiter(input, i) == -1)
			return (1);
	}
	else
		skip_until_operator_or_space(input, i);
	delimiter = ft_substr(input, start, *i - start);
	if (!delimiter)
		return (1);
	heredoc_token->heredoc_delimiter = delimiter;
	return (0);
}

void	process_heredocs(void)
{
	t_command	*cmd;
	t_token		*token;

	cmd = g_data.commands;
	while (cmd)
	{
		token = cmd->token_list;
		while (token)
		{
			if (token->is_heredoc && !cmd->is_recalled)
			{
				if (read_heredoc_content(token))
				{
					g_data.error_flag = 1;
					return ;
				}
			}
			token = token->next;
		}
		cmd = cmd->next;
	}
}

int	process_heredoc_delimiter(char *input, int *i, t_token *heredoc_token)
{
	char	*delimiter;
	int		start;

	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	if (!input[*i])
		return (syntax_error_newline());
	start = *i;
	if (input[*i] == '\'' || input[*i] == '\"')
	{
		if (process_quoted_delimiter(input, i) == -1)
			return (1);
	}
	else
		skip_until_operator_or_space(input, i);
	delimiter = ft_substr(input, start, *i - start);
	if (!delimiter)
		return (1);
	heredoc_token->heredoc_delimiter = delimiter;
	if (read_heredoc_content(heredoc_token))
		return (1);
	return (0);
}

int	process_quoted_delimiter(char *input, int *i)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
		(*i)++;
	else
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote in "
			"heredoc delimiter\n", STDERR_FILENO);
		return (-1);
	}
	return (0);
}

void	skip_until_operator_or_space(char *input, int *i)
{
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator_char(input[*i]))
		(*i)++;
}

int	syntax_error_newline(void)
{
	ft_putstr_fd("minishell: syntax error near unexpected token "
		"`newline'\n", STDERR_FILENO);
	return (1);
}

int read_heredoc_content(t_token *heredoc_token)
{
    char    *line;
    int     fd;
    char    *tmp_filename;
    int     delimiter_quoted;

    delimiter_quoted = check_delimiter_quotes(heredoc_token);
    tmp_filename = generate_temp_filename();
    if (!tmp_filename)
        return (1);
    fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd < 0)
        return (heredoc_open_error(tmp_filename));
    while (1)
    {
        line = readline("heredoc> ");
        if (!line || handle_heredoc_line(line, heredoc_token, fd, delimiter_quoted))
            break;
    }
    close(fd);
    heredoc_token->heredoc_file = tmp_filename;
    return (0);
}

int	check_delimiter_quotes(t_token *heredoc_token)
{
	char	*delimiter;
	char	*unquoted_delimiter;

	delimiter = heredoc_token->heredoc_delimiter;
	if (delimiter[0] == '\'' || delimiter[0] == '\"')
	{
		unquoted_delimiter = ft_substr(delimiter, 1, ft_strlen(delimiter) - 2);
		if (!unquoted_delimiter)
			return (-1);
		free(heredoc_token->heredoc_delimiter);
		heredoc_token->heredoc_delimiter = unquoted_delimiter;
		return (1);
	}
	return (0);
}

int	heredoc_open_error(char *tmp_filename)
{
	perror("minishell: heredoc");
	free(tmp_filename);
	return (1);
}

int handle_heredoc_line(char *line, t_token *heredoc_token, int fd, int delimiter_quoted)
{
    if (append_heredoc_full_input(line))
    {
        free(line);
        return (1);
    }
    if (ft_strcmp(line, heredoc_token->heredoc_delimiter) == 0)
    {
        free(line);
        return (1);
    }
    if (!delimiter_quoted)
    {
        if (expand_and_write_line(line, fd))
        {
            free(line);
            return (1);
        }
    }
    else
    {
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
    }
    free(line);
    return (0);
}



int	append_heredoc_full_input(char *line)
{
	g_data.full_input = ft_strjoin_and_free_first(
			g_data.full_input, "\nheredoc> ");
	if (!g_data.full_input)
	{
		ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
		return (1);
	}
	g_data.full_input = ft_strjoin_and_free_first(g_data.full_input, line);
	if (!g_data.full_input)
	{
		ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int expand_and_write_line(char *line, int fd)
{
    int     var_not_found_flag;
    char    *temp;

    var_not_found_flag = 0;
    temp = expand_variables_in_token(line, &var_not_found_flag);
    if (!temp)
        return (1);
    write(fd, temp, ft_strlen(temp));
    write(fd, "\n", 1);
    free(temp);
    return (0);
}




char	*generate_temp_filename(void)
{
	static int	counter = 0;
	char		*filename;
	char		*counter_str;

	counter_str = ft_itoa(counter++);
	if (!counter_str)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", counter_str);
	free(counter_str);
	return (filename);
}



