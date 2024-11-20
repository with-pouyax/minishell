#include "../minishell.h"

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

int	read_heredoc_content(t_shell_data *shell, t_token *heredoc_token)
{
	char *line;
	int fd;
	char *tmp_filename;
	int delimiter_quoted;

	delimiter_quoted = check_delimiter_quotes(heredoc_token); // Check if the delimiter is quoted
	tmp_filename = generate_temp_filename(); // Generate a temporary filename
	if (!tmp_filename)
		return (1);
	fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600); // Open the temporary file
	if (fd < 0)
		return (heredoc_open_error(tmp_filename));
	while (1) // Loop until we reach the delimiter
	{
		line = readline("> "); // Read a line from the user
		if (!line || handle_heredoc_line(shell, line, heredoc_token, fd, delimiter_quoted)) // If the line is NULL or the line is the delimiter, break the loop
			break;
	}

	close(fd); // Close the file
	heredoc_token->heredoc_file = tmp_filename; // Store the temporary filename in the heredoc_token
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
                if (read_heredoc_content(shell, token)) // Read the heredoc content
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
