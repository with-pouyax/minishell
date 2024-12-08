#include "../minishell.h"

int	calc_pipe_nb(t_shell_data *shell)
{
	char	*full_input;
	int		pipe_nb;

	full_input = shell->full_input;
	pipe_nb = 0;
	while (*full_input)
	{
		if (*full_input == '|')
			pipe_nb++;
		full_input++;
	}
	return (pipe_nb);
}

int	calc_cmds_nb(t_shell_data *shell)
{
	char	*input;
	int		cmd_count;
	int		i;

	input = shell->full_input;
	cmd_count = 0;
	i = 0;
	if (!input || !*input)
		return (0);
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (input[i] && input[i] != '|')
		{
			cmd_count++;
			while (input[i] && input[i] != '|')
				i++;
		}
		if (input[i] == '|')
			i++;
	}
	return (cmd_count);
}

void	init_shell(t_shell_data *shell)
{
    shell->input = NULL;
    shell->full_input = NULL;
    shell->exit_status = 0;
    shell->commands = NULL;
    shell->error_flag = 0;
    shell->signal_status = 0;
    shell->in_single_quote = 0;
    shell->in_double_quote = 0;
    shell->prev_char = '\0';
    shell->var_not_found_flag = 0;
	shell->prev_dir = getcwd(NULL, 0);
	if (!shell->prev_dir)
	{
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	else
		free(shell->prev_dir);
}
