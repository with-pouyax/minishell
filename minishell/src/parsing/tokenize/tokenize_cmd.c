# include "../../minishell.h"

void	add_command_to_list(t_shell_data *shell, t_command **last_cmd, \
t_command *cmd)
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
    {
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
        return (NULL);
    }
    cmd->command_string = cmd_str;
    cmd->index = index;
    cmd->is_recalled = 0;
    cmd->token_list = NULL;
    cmd->redirections = NULL;
    cmd->token_index = 0;
    cmd->next = NULL;
    return (cmd);
}
/*****************************************************************************/
// 🎯 Purpose  :  turn input into commands
/*****************************************************************************/
//
// 🔹 Parameters:                                                             
//     🏷  input -> user input
//     🏷  i -> the index of the first non-space character in the input                              
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:                                                                  
//     1- using a while loop we iterate over the input characters as long as
//        the current character is not a pipe.
//        a- if the current character is a single or double quote we skip the
//           quoted word, because we don't want to count the pipes inside the
//           quotes as a pipe.
//        b- if the current character is not a quote we increment the index.
//     2- after our stop, we check if the current character is a pipe and the
//        next character is also a pipe, we print an error message 
//        and return -1
//     3- if 1st character after the loop stop is a space, using a while loop
//        we skip the spaces until we find a non-space character.
//     4- if the 1st character after the loop stop is a pipe, we print an error
//        message and return -1.
/******************************************************************************/
int extract_command_string(char *input, int i)
{
    while (input[i] && input[i] != '|')
	{
		if (input[i] == '\'' || input[i] == '\"')
			i = skip_quotes(input, i);
		else
			i++;
	}
    if (input[i] == '|' && input[i + 1] == '|')
        return (ft_putstr_fd("minishell: syntax error near \
        unexpected token `|'\n", STDERR_FILENO), -1);
    if (input[i] && input[i + 1] == ' ')
    {
        i++;
        while (input[i] && input[i] == ' ')
            i++;
        if (input[i] == '|')
            return (ft_putstr_fd("minishell: syntax error near \
            unexpected token `|'\n", STDERR_FILENO), -1);
    }    
    return (i);
}
