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
	if (!cmd || tokenize_command(shell, cmd))
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

void    append_end_token(t_shell_data *shell)
{
    t_command   *cmd;
    t_token     *last_token;
    char        *end_token_str;
    int         end_index;

    if (!shell->commands)
        return;

    // Traverse to the last command
    cmd = shell->commands;
    while (cmd->next)
        cmd = cmd->next;

    // Traverse to the last token in the last command
    last_token = cmd->token_list;
    if (last_token)
    {
        while (last_token->next)
            last_token = last_token->next;
        end_index = last_token->index + 1;
    }
    else
    {
        // If there are no tokens, start index at 0
        end_index = 0;
    }

    // Create a new token with value "\0"
    end_token_str = ft_strdup("\0"); // Alternatively, use a unique string like "<END>"
    if (!end_token_str)
    {
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
        free_commands(shell);
        shell->commands = NULL;
        return;
    }

    // Add the '\0' token to the token list
    if (add_token(end_token_str, &cmd->token_list, &end_index, 0))
    {
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
        free(end_token_str);
        free_commands(shell);
        shell->commands = NULL;
        return;
    }

    // Traverse to the newly added '\0' token
    last_token = cmd->token_list;
    while (last_token->next)
        last_token = last_token->next;

    // Set the is_end flag
    last_token->is_end = 1;
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
	else
    {
        // Append the '\0' token to the last command
        append_end_token(shell);
    }
	
}
