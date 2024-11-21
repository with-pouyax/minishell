#include "../../minishell.h"

int	process_input_segment(t_shell_data *shell, int *i, int *cmd_index, t_command **last_cmd)
{
	int			start;
	char		*cmd_str;
	t_command	*cmd;

	start = *i;
	*i = extract_command_string(shell->input, *i);                       // here we find the end index of the command, before the pipe
	cmd_str = ft_substr(shell->input, start, *i - start);                // we extract the command string from the input and store it in cmd_str
	if (!cmd_str)
	{
		shell->error_flag = 2;
		return (1);
	}
	cmd = create_command(shell ,cmd_str, (*cmd_index)++); // we create a new command struct and store it in cmd
	if (!cmd || tokenize_command(cmd))
	{
		free(cmd_str);
		free(cmd);
		shell->error_flag = 2;
		return (1);
	}
	add_command_to_list(shell, last_cmd, cmd); // we add the command to the list of commands,
	if (shell->input[*i] == '|')
		(*i)++;
	return (0);
}

void	split_cmd_tokenize(t_shell_data *shell)
{
	int			i;
	int			cmd_index;
	t_command	*last_cmd;

	i = 0;
	cmd_index = 0;
	last_cmd = NULL;
	shell->error_flag = 0;
	if (!shell->input)
        return;
	while (shell->input[i])
	{
		skip_spaces(shell, &i);
		if (process_input_segment(shell, &i, &cmd_index, &last_cmd)) // here we save each command in a linked list
			break ;
	}
	if (shell->error_flag)
	{
		free_commands(shell);
		shell->commands = NULL;
		handle_tokenization_error(shell, shell->error_flag);
	}
}