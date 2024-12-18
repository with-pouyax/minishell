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

int	split_cmd_tokenize(t_shell_data *shell)
{
	int		i;
	int		cmd_index;
	t_command	*last_cmd;

	i = 0;
	cmd_index = 0;
	last_cmd = NULL;
	shell->error_flag = 0;
	if (!shell->input) 														      //if there is no input means the user pressed ctrl+d
		return (1);
	while (shell->input[i])                                                       //input is a string and i is the index, so here we loop through each character of the input
	{
		skip_spaces(shell, &i);													  //first we skip the spaces
		if (process_input_segment(shell, &i, &cmd_index, &last_cmd) > 0)		  //[x]we create a list of commands separated by pipes and store it in shell->commands
			return (1);
	}
	if (shell->error_flag)
		handle_tokenization_failure(shell);
	else																		 //if there are no errors in creating the list of commands
		count_commands_and_pipes(shell);
	return (0);
}
