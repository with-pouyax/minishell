#include "minishell.h"

int	get_heredoc_delimiter(t_shell_data *shell, char *input, int *i, t_token *heredoc_token)
{
	char	*delimiter;
	int		start;

	while (input[*i] && ft_isspace(input[*i])) // ok
		(*i)++;
	if (!input[*i])
		return (syntax_error_newline(shell)); // ok
	start = *i;
	if (input[*i] == '\'' || input[*i] == '\"')
	{
		if (process_quoted_delimiter(input, i) == -1) // ok
			return (1);
	}
	else
		skip_until_operator_or_space(input, i); // ok
	delimiter = ft_substr(input, start, *i - start); // ok
	if (!delimiter)
		return (1);
	heredoc_token->heredoc_delimiter = delimiter;
	return (0);
}

void process_heredocs(t_shell_data *shell)
{
    t_command *cmd;
    t_token *token;

    cmd = shell->commands;
    while (cmd) // Loop through all commands
    {
        token = cmd->token_list;
        while (token) // Loop through all tokens in the command
        {
            if (token->is_heredoc) // If the token is a heredoc token
            {
                if (read_heredoc_content(shell, token)) //ok
                {
                    shell->error_flag = 1;
                    return;
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

int	syntax_error_newline(t_shell_data *shell)
{
	ft_putstr_fd("minishell: syntax error near unexpected token "
		"`newline'\n", STDERR_FILENO);
	return (1);
}
char *get_line_from_input(char *input, int *index)
{
    int start = *index;
    while (input[*index] && input[*index] != '\n')
        (*index)++;
    char *line = ft_substr(input, start, *index - start);
    if (input[*index] == '\n')
        (*index)++;
    return line;
}



int	read_heredoc_content(t_shell_data *shell, t_token *heredoc_token)
{
	char *line;
	int fd;
	char *tmp_filename;
	int delimiter_quoted;

	delimiter_quoted = check_delimiter_quotes(heredoc_token); //ok
	tmp_filename = generate_temp_filename(shell); // ok
	if (!tmp_filename)
		return (1);
	fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600); // Open the temporary file
	if (fd < 0)
		return (heredoc_open_error(tmp_filename));//ok
	while (1) // Loop until we reach the delimiter
	{
		line = readline("> "); // Read a line from the user
		if (!line || handle_heredoc_line(shell, line, heredoc_token, fd, delimiter_quoted)) //ok
			break;
	}

	close(fd); // Close the file
	heredoc_token->heredoc_file = tmp_filename; // Store the temporary filename in the heredoc_token
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

int handle_heredoc_line(t_shell_data *shell, char *line, t_token *heredoc_token, int fd, int delimiter_quoted)
{
    
    if (ft_strcmp(line, heredoc_token->heredoc_delimiter) == 0) // If the line is the delimiter, break the loop
    {
        free(line);
        return (1);
    }
    if (!delimiter_quoted) // If the delimiter is not quoted
    {
        if (expand_and_write_line(shell, line, fd)) // Expand the variables in the line and write the line to the file
        {
            free(line);
            return (1);
        }
    }
    else // If the delimiter is quoted
    {
        write(fd, line, ft_strlen(line)); // Write the line to the file 
        write(fd, "\n", 1);
    }
    free(line);
    return (0);
}

int expand_and_write_line(t_shell_data *shell, char *line, int fd)
{
    int     var_not_found_flag;
    char    *temp;

    var_not_found_flag = 0;
    temp = expand_variables_in_token(shell, line, &var_not_found_flag); //ok
    if (!temp)
        return (1);
    write(fd, temp, ft_strlen(temp)); // Write the expanded line to the file
    write(fd, "\n", 1); // Write a newline character to the file
    free(temp); // Free the expanded line
    return (0);
}




char	*generate_temp_filename(t_shell_data *shell)
{
	char		*filename;
	char		*counter_str;

	counter_str = ft_itoa(shell->heredoc_counter++); // Convert the counter to a string
	if (!counter_str)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", counter_str); // Join the prefix and the counter string
	free(counter_str);
	return (filename);
}



