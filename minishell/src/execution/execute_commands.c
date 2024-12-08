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
		if (redir->type == REDIR_INPUT)
			fd_input = open_input_file(shell, redir, fd_input);
		else if (redir->type == REDIR_OUTPUT)
			fd_output = open_output_file(shell, redir, fd_output);
		else if (redir->type == REDIR_APPEND)
			fd_output = open_append_file(shell, redir, fd_output);
		redir = redir->next;
	}
	if (shell->exit_status != 0 && shell->last_error_file)
		write_error(shell->last_error_file, strerror(shell->exit_status));
	if (fd_input == -1 || fd_output == -1)
		return (EXIT_FAILURE);
	else
		return (EXIT_SUCCESS);
}
/*
Restore the original stdin and stdout
Close the saved file descriptors

*/
void restore_org_in_out(int saved_stdin, int saved_stdout)
{
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
}

/*
set_redir() :
    handle the redirection operators in a program. 
    Redirections determine how input and output are handled for 
    commands, such as reading from a file or writing to a file.
    -- fd_i and fd_o :
    represent the file descriptors for input (<) and output (> or >>) files.
    They are initialized to -2, a special value indicating no file is open yet.
*/
void	exec_cmd(t_shell_data *shell, t_command *cmds, int index)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		perror("dup failed");
		return ;
	}
	set_redirection(shell, cmds->redirections);
	if (shell->exit_status == EXIT_SUCCESS)
	{
		set_pipes(shell, cmds->redirections, index);
		cleanup_heredocs(cmds->redirections);
		forking(shell, cmds);
	}
	restore_org_in_out(saved_stdin, saved_stdout);
}

void	execution(t_shell_data *shell)
{
	t_command	*cmd;
	int		i;

	i = 0;
	cmd = shell->commands;
	shell->pipes = init_pipes(shell->cmds_nb);
	while (i < shell->cmds_nb)
	{
		exec_cmd(shell, cmd, i);
		cmd = cmd->next;
		i++;
	}
	close_all_pipes(shell->pipes, shell->cmds_nb);
	execute_parent(shell);
	free_pipes(shell->pipes, shell->cmds_nb);
}
