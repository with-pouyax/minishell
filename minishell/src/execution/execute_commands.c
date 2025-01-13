#include "../minishell.h"

/*
Restore the original stdin and stdout
Close the saved file descriptors

*/
// void	restore_org_in_out(int saved_stdin, int saved_stdout)
// {
// 	dup2(saved_stdin, STDIN_FILENO);
// 	dup2(saved_stdout, STDOUT_FILENO);
// 	close(saved_stdin);
// 	close(saved_stdout);
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
	shell->saved_stdin = dup(STDIN_FILENO);
	shell->saved_stdout = dup(STDOUT_FILENO);
	if (shell->saved_stdin == -1 || shell->saved_stdout == -1)
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
	// restore_org_in_out(shell->saved_stdin, shell->saved_stdout);
	dup2(shell->saved_stdin, STDIN_FILENO);
	dup2(shell->saved_stdout, STDOUT_FILENO);
	close(shell->saved_stdin);
	close(shell->saved_stdout);

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
