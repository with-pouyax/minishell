#include "minishell.h"

void free_envp(t_shell_data *shell)
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

void cleanup(t_shell_data *shell)
{
    free(shell->input);
    shell->input = NULL;
    free(shell->full_input);
    shell->full_input = NULL;
    free_commands(shell);
    free_envp(shell);
}

void	handle_tokenization_error(t_shell_data *shell, int error_flag)
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
		cleanup(shell); // ok
		exit(EXIT_FAILURE); // ok
	}
}

int	process_input_segment(t_shell_data *shell, int *i, int *cmd_index, t_command **last_cmd)
{
	int			start;
	char		*cmd_str;
	t_command	*cmd;

	start = *i;
	*i = extract_command_string(shell->input, *i); // ok
	cmd_str = ft_substr(shell->input, start, *i - start); // ok
	if (!cmd_str)
	{
		shell->error_flag = 2;
		return (1);
	}
	cmd = create_command(cmd_str, (*cmd_index)++); // ok
	if (!cmd || tokenize_command(shell, cmd)) // ok
	{
		free(cmd_str);
		free(cmd);
		shell->error_flag = 2;
		return (1);
	}
	add_command_to_list(shell, last_cmd, cmd); //ok
	if (shell->input[*i] == '|')
		(*i)++;
	return (0);
}

void	tokenize_input(t_shell_data *shell)
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

	while (shell->input[i]) // Loop through the input string
	{
		skip_spaces(&i); // Skip spaces
		if (process_input_segment(shell, &i, &cmd_index, &last_cmd)) //ok
			break ;
	}
	if (shell->error_flag)
	{
		free_commands(shell); //ok
		shell->commands = NULL;
		handle_tokenization_error(shell, shell->error_flag); //ok
	}
	// else
    // {
    //     printf("tokenize_input: Successfully parsed %d commands.\n", cmd_index); // ##debug print
    // }
}

void	skip_spaces(int *i)
{
	while (g_data.input[*i] && ft_isspace(g_data.input[*i]))
		(*i)++;
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

void	add_command_to_list(t_shell_data *shell, t_command **last_cmd, t_command *cmd)
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
	g_data.commands = NULL;
	g_data.error_flag = error_flag;
}
