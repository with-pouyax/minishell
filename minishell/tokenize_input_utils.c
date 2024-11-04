#include "minishell.h"

void	cleanup(void)
{
	free(g_data.input);
	g_data.input = NULL;
	free(g_data.full_input);
	g_data.full_input = NULL;
	free_commands();
}

void	handle_tokenization_error(int error_flag)
{
	if (error_flag == 1)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
			STDERR_FILENO);
		g_data.exit_status = 2;
	}
	else if (error_flag == 2)
	{
		ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
		cleanup();
		exit(EXIT_FAILURE);
	}
}

int	process_input_segment(int *i, int *cmd_index, t_command **last_cmd)
{
	int			start;
	char		*cmd_str;
	t_command	*cmd;

	start = *i;
	*i = extract_command_string(g_data.input, *i); //
	cmd_str = ft_substr(g_data.input, start, *i - start);
	if (!cmd_str)
	{
		g_data.error_flag = 2;
		return (1);
	}
	cmd = create_command(cmd_str, (*cmd_index)++);
	if (!cmd || tokenize_command(cmd))
	{
		free(cmd_str);
		free(cmd);
		g_data.error_flag = 2;
		return (1);
	}
	add_command_to_list(last_cmd, cmd);
	if (g_data.input[*i] == '|')
		(*i)++;
	return (0);
}

void	tokenize_input(void)
{
	int			i;
	int			cmd_index;
	t_command	*last_cmd;

	i = 0;
	cmd_index = 0;
	last_cmd = NULL;
	g_data.error_flag = 0;
	while (g_data.input[i]) // Loop through the input string
	{
		skip_spaces(&i); // Skip spaces
		if (process_input_segment(&i, &cmd_index, &last_cmd)) // Process the input segment
			break ;
	}
	if (g_data.error_flag)
	{
		free_commands();
		g_data.commands = NULL;
		handle_tokenization_error(g_data.error_flag);
	}
}

void	skip_spaces(int *i)
{
	while (g_data.input[*i] && ft_isspace(g_data.input[*i]))
		(*i)++;
}

int	extract_command_string(char *input, int i)
{
	while (input[i] && input[i] != '|')
	{
		if (input[i] == '\'' || input[i] == '\"')
			i = skip_quotes(input, i);
		else
			i++;
	}
	return (i);
}

void	add_command_to_list(t_command **last_cmd, t_command *cmd)
{
	if (!g_data.commands)
		g_data.commands = cmd;
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
