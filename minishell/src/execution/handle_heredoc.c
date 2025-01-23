#include "../../include/minishell.h"
/*
  // Create a pipe to pass heredoc input to the next command
    if (pipe(pipe_fd) == -1) {
 // Redirect STDIN to the heredoc file so the content can be used by the
    next command
    if (dup2(heredoc_fd, STDIN_FILENO) == -1)
 // Redirect STDOUT to the pipe so the content can be piped to the next process
    if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
*/
void	handle_heredoc(t_shell_data *shell, t_redirection *redir)
{
	int	heredoc_fd;

	heredoc_fd = open(redir->heredoc_file, O_RDONLY);
	if (heredoc_fd == -1)
	{
		perror("Failed to open heredoc file");
		shell->exit_status = EXIT_FAILURE;
		return ;
	}
	if (dup2(heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("Failed to redirect STDIN to heredoc");
		close(heredoc_fd);
		shell->exit_status = EXIT_FAILURE;
		return ;
	}
	close(heredoc_fd);
}

/*
unlink(redir->heredoc_file); // Remove the temporary file
*/
void	cleanup_heredocs(t_redirection *redir)
{
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC && redir->heredoc_file)
		{
			unlink(redir->heredoc_file);
		}
		redir = redir->next;
	}
}
