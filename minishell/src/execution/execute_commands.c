#include "../minishell.h"
void track_fds(const char *stage) {
    printf("\n[FD Tracking at %s]\n", stage);
    for (int fd = 0; fd < 1024; fd++) {
        if (fcntl(fd, F_GETFD) != -1) {  // FD is open
            printf("FD %d is open\n", fd);
        }
    }
    printf("\n");
}

void close_open_redirection_fds(t_redirection *redir)
{
    while (redir)
	{
        if (redir->fd > 2)
		{  // Ensure not closing stdin, stdout, stderr
            close(redir->fd);
        }
        redir = redir->next;
    }
}
/*
Restore the original stdin and stdout
Close the saved file descriptors

*/
// void	restore_org_in_out(int saved_stdin, int saved_stdout)
// {
// }

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
	track_fds("Before setting redirection");
	if (cmds->redirections)
		set_redirection(shell, cmds->redirections);
	track_fds("After setting redirection");
	if (shell->exit_status == EXIT_SUCCESS)
	{
		if (cmds->redirections || shell->cmds_nb > 1)
			set_pipes(shell, cmds->redirections, index);
		cleanup_heredocs(cmds->redirections);
		forking(shell, cmds);
	}
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	// close_open_redirection_fds(cmds->redirections);
	track_fds("After close fd");

}


void	execution(t_shell_data *shell)
{
	t_command	*cmd;
	int			i;

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
