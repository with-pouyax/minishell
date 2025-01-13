#include "../minishell.h"

int	has_redirs(t_redirection *redir, t_redirection_type type )
{
	while (redir)
	{
		if (redir->type == type)
			return (1);
		redir = redir->next;
	}
	return (0);
}

static int handle_redir(t_shell_data *shell, t_redirection *redir,
                int *fd_input, int *fd_output)
{
    int fd;

    // Determine which FD to manage
    if (redir->type == REDIR_INPUT)
        fd = *fd_input;  // Use fd_input for input redirection
    else
        fd = *fd_output; // Use fd_output for output redirection

    // Close the previous FD if it is open
    if (fd >= 0)
        close(fd);

    // Open the new file
    if (redir->type == REDIR_INPUT)
        fd = open_input_file(shell, redir, fd);  // Open input file and assign fd
    else if (redir->type == REDIR_OUTPUT)
        fd = open_output_file(shell, redir, fd);  // Open output file and assign fd
    else if (redir->type == REDIR_APPEND)
        fd = open_append_file(shell, redir, fd);  // Open file for append and assign fd

    // Check for errors
    if (fd == -1)
    {
        write_error(shell->last_error_file, strerror(shell->exit_status));
        shell->exit_status = 1;
        return (EXIT_FAILURE);
    }

    // Assign the file descriptor to the redirection object
    redir->fd = fd;

    // Update the fd_input or fd_output pointer depending on redirection type
    if (redir->type == REDIR_INPUT)
        *fd_input = fd;  // Set the input file descriptor for future use
    else
        *fd_output = fd;  // Set the output file descriptor for future use

    return (EXIT_SUCCESS);
}



static int	finalize_redir(t_shell_data *shell, int fd_input, int fd_output)
{
	if (shell->exit_status != 0 && shell->last_error_file)
		write_error(shell->last_error_file, strerror(shell->exit_status));
	if (fd_input == -1 || fd_output == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

// use of fds here : to check If any of the open_file() returned -1,
// it means there was an error
// Opens all files. Only the last of its type is left open, others are closed.
int	open_all_files(t_shell_data *shell, t_redirection *redir)
{
	int	fd_input;
	int	fd_output;

	fd_input = -2;
	fd_output = -2;
	shell->last_error_file = NULL;
	while (redir)
	{
		if (handle_redir(shell, redir, &fd_input, &fd_output) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		redir = redir->next;
	}
	return (finalize_redir(shell, fd_input, fd_output));
}
