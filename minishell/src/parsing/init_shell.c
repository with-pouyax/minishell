#include "minishell.h"

static int	calc_pipe_nb(t_shell_data *shell)
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
static int calc_cmds_nb(t_shell_data *shell)
{
    char *input = shell->full_input; // Get the full input
    int cmd_count = 0;
    int i = 0;

    if (!input || !*input) // Check if input is empty or NULL
        return 0;

    while (input[i])
    {
        // Skip leading spaces around commands
        while (input[i] == ' ' || input[i] == '\t')
            i++;

        // If we're at the start of a command, increment cmd_count
        if (input[i] && input[i] != '|')
        {
            cmd_count++;
            // Move to the end of the current command
            while (input[i] && input[i] != '|')
                i++;
        }

        // Skip over the pipe symbol
        if (input[i] == '|')
            i++;
    }

    return cmd_count;
}

void	init_shell(t_shell_data *shell)
{
	shell->input = NULL;
	shell->full_input = NULL;
	shell->exit_status = 0;
	shell->commands = NULL;
	shell->error_flag = 0;
	shell->signal_status = 0;
}
