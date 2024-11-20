#include "minishell.h"

void free_envp(void)
{
    int i;

    if (!shell->envp)
        return;

    for (i = 0; shell->envp[i]; i++)
    {
        free(shell->envp[i]);
    }
    free(shell->envp);
    shell->envp = NULL;
}

void cleanup(void)
{
    free(shell->input);
    shell->input = NULL;
    free(shell->full_input);
    shell->full_input = NULL;
    free_commands();
    free_envp();
}

void	handle_tokenization_error(int error_flag)
{
	if (error_flag == 1)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
			STDERR_FILENO);
		shell->exit_status = 2;
	}
	else if (error_flag == 2)
	{
		ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
		cleanup();
		exit(EXIT_FAILURE);
	}
}


int	extract_command_string(char *input, int i)
{
	while (input[i] && input[i] != '|') // Loop through the input string until we find a pipe
	{
		if (input[i] == '\'' || input[i] == '\"') // If we find a quote
			i = skip_quotes(input, i); // Skip quotes
		else
			i++;
	}
	return (i);
}

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
	cmd = create_command(cmd_str, (*cmd_index)++); // we create a new command struct and store it in cmd
	if (!cmd || tokenize_command(cmd))
	{
		free(cmd_str);
		free(cmd);
		shell->error_flag = 2;
		return (1);
	}
	add_command_to_list(last_cmd, cmd); // we add the command to the list of commands,
	if (shell->input[*i] == '|')
		{
			(*i)++;
		}
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
		free_commands();
		shell->commands = NULL;
		handle_tokenization_error(shell->error_flag);
	}
	// else
    // {
    //     printf("tokenize_input: Successfully parsed %d commands.\n", cmd_index); // ##debug print
    // }
}



void	add_command_to_list(t_command **last_cmd, t_command *cmd)
{
	if (!shell->commands)
		shell->commands = cmd;
	else
		(*last_cmd)->next = cmd;
	*last_cmd = cmd;
}

void	tokenize_input_error(int error_flag)
{
	free_commands();
	shell->commands = NULL;
	shell->error_flag = error_flag;
}
