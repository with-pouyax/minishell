#include "../../minishell.h"

int	process_input_segment(t_shell_data *shell, int *i, int *cmd_index,
				t_command **last_cmd)
{
	int		start;
	char		*cmd_str;
	t_command	*cmd;

	start = *i;
	*i = extract_command_string(shell->input, *i);
	if (*i == -1)
	{
		shell->error_flag = 1;
		return (1);
	}
	cmd_str = ft_substr(shell->input, start, *i - start);
	if (!cmd_str)
	{
		shell->error_flag = 2;
		return (1);
	}
	int len = ft_strlen(cmd_str);
	while (len > 0 && ft_isspace(cmd_str[len - 1]))
		cmd_str[--len] = '\0';
	cmd = create_command(shell, cmd_str, (*cmd_index)++);
	if (!cmd || tokenize_command(shell, cmd))
	{
		free(cmd_str);
		free(cmd);
		shell->error_flag = 1;
		return (1);
	}
	add_command_to_list(shell, last_cmd, cmd);
	if (shell->input[*i] == '|')
		(*i)++;
	return (0);
}
