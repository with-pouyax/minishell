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
	while (redir)
	{
		if (redir->type == REDIR_INPUT)
			fd_input = open_input_file(shell, redir, fd_input);
		else if (redir->type == REDIR_OUTPUT)
			fd_output = open_output_file(redir, fd_output);
		else if (redir->type == REDIR_APPEND)
			fd_output = open_append_file(redir, fd_output);
		redir = redir->next;
	}
	return (EXIT_SUCCESS);
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

	printf("the cmd is executing: %s\n", cmds->command_string);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		perror("dup failed");
		return ;
	}
	set_redirection(shell, cmds->redirections);
	set_pipes(shell, cmds->redirections, index);
	if (shell->exit_status == EXIT_SUCCESS)
	{
		if (cmds->token_list->is_int)
			execute_internal_commands(shell, cmds);
		else
			execute_external_commands(shell, cmds);
	}
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
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
