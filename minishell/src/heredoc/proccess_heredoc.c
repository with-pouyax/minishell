#include "../minishell.h"

int handle_heredoc_line(t_shell_data *shell, char *line, t_redirection *redir, int fd, int delimiter_quoted)
{
    if (ft_strcmp(line, redir->delimiter) == 0)
    {
        free(line);
        return (1);
    }
    if (!delimiter_quoted)
    {
        if (expand_and_write_line(shell, line, fd))
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
/*read_heredoc_content
- Implements the heredoc reading loop:
- Reads lines from the user (readline("> ")).
- Compares each line with the delimiter.
- Expands variables unless the delimiter is quoted.
- Writes expanded or raw content into a temporary file.
- Stores the path of the temporary file in heredoc_token.
*/

<<<<<<< HEAD
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
/*
process_heredocs
- Iterates through all commands in the shell's commands list.
- Processes each heredoc token by calling read_heredoc_content.

*/
=======
>>>>>>> d8a31fee03d36587b120b4562ac5b45af3b4a40d

int read_heredoc_content(t_shell_data *shell, t_redirection *redir)
{
    char *line;
    int fd;
    char *tmp_filename;
    int delimiter_quoted;

    delimiter_quoted = check_delimiter_quotes(redir); // Adjusted to use redirection
    tmp_filename = generate_temp_filename();
    if (!tmp_filename)
        return (1);
    fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd < 0)
        return (heredoc_open_error(tmp_filename));
    while (1)
    {
        line = readline("> ");
        if (!line || handle_heredoc_line(shell, line, redir, fd, delimiter_quoted))
            break;
    }
    close(fd);
    redir->heredoc_file = tmp_filename; // Store the filename in redirection
    return (0);
}
