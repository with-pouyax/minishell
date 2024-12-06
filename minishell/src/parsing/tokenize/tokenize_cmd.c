# include "../../minishell.h"

void	add_command_to_list(t_shell_data *shell, t_command **last_cmd, t_command *cmd)
{
	if (!shell->commands)
		shell->commands = cmd;
	else
		(*last_cmd)->next = cmd;
	*last_cmd = cmd;
}

t_command	*create_command(t_shell_data *shell, char *cmd_str, int index)
{
    t_command *cmd;

    (void)shell;
    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);
    cmd->command_string = cmd_str;
    cmd->index = index;
    cmd->is_recalled = 0;
    cmd->token_list = NULL;
    cmd->redirections = NULL;
    cmd->token_index = 0; // Initialize token_index to 0
    cmd->next = NULL;
    return (cmd);
}

int extract_command_string(char *input, int i)
{
    while (input[i] && input[i] != '|') // Loop through the input string until we find a pipe
	{
		if (input[i] == '\'' || input[i] == '\"') // If we find a quote
			i = skip_quotes(input, i); // Skip quotes
		else
			i++;
	}
    if (input[i] == '|' && input[i + 1] == '|')
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
        return (-1);
    }
    if (input[i] && input[i + 1] == ' ')
    {
        i++;
        while (input[i] && input[i] == ' ')
            i++;
        // if(input[i] )
        //     printf("*****%c\n", input[i]);
        if (input[i] == '|')
        {
            ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
            return (-1);
        }
    }    
    return (i);
}