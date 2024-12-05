#include "../../minishell.h"

void	count_commands_and_pipes(t_shell_data *shell)
{
	int			cmd_count;
	t_command	*cmd;

	cmd_count = 0;
	cmd = shell->commands;
	while (cmd)
	{
		cmd_count++;
		cmd = cmd->next;
	}
	shell->cmds_nb = cmd_count;
	shell->pipe_nb = cmd_count - 1;
}

void	handle_tokenization_failure(t_shell_data *shell)
{
	free_commands(shell);
	shell->commands = NULL;
	handle_tokenization_error(shell, shell->error_flag);
}

void	split_cmd_tokenize(t_shell_data *shell)
{
	int		i;
	int		cmd_index;
	t_command	*last_cmd;

	i = 0;
	cmd_index = 0;
	last_cmd = NULL;
	shell->error_flag = 0;
	if (!shell->input)
		return ;
	while (shell->input[i])
	{
		skip_spaces(shell, &i);
		if (process_input_segment(shell, &i, &cmd_index, &last_cmd))
			break ;
	}
	if (shell->error_flag)
		handle_tokenization_failure(shell);
	else
	{
		// append_end_token(shell);
		count_commands_and_pipes(shell);
	}
}
