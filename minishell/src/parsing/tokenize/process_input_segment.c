#include "../../minishell.h"

void	trim_trailing_spaces(char *cmd_str)
{
	int	len;

	if (!cmd_str)
		return ;
	len = ft_strlen(cmd_str);
	while (len > 0 && ft_isspace(cmd_str[len - 1]))
		cmd_str[--len] = '\0';
}

int	handle_command_creation_error(t_shell_data *shell,
				char *cmd_str, t_command *cmd)
{
	free(cmd_str);
	free(cmd);
	shell->error_flag = 1;
	return (1);
}

int process_input_segment(t_shell_data *shell, int *i, int *cmd_index,
                          t_command **last_cmd)
{
    int start;
    char *cmd_str;
    t_command *cmd;

    start = *i;
    (void)start;
    *i = extract_command_string(shell->input, *i);                   //here we find the command in the input string separated by pipes
    if (*i == -1)                                                
        return (shell->error_flag = 1, 1);
    cmd_str = ft_substr(shell->input, start, *i - start);           //we put the command, we found in the input, in cmd_str
    if (!cmd_str)
        return (shell->error_flag = 2, 1);
    trim_trailing_spaces(cmd_str);                                 //if there are spaces at the end of the command, we remove them
    cmd = create_command(shell, cmd_str, (*cmd_index)++);          // here we create a command struct for the command we found          
    if (!cmd || tokenize_command(shell, cmd))                      // we tokenize the command we found
        return (handle_command_creation_error(shell, cmd_str, cmd));
    add_command_to_list(shell, last_cmd, cmd);                     //we store the list of commands
    if (shell->input[*i] == '|')                                   //if the next character aftr the command we just created is a pipe, we increment i
        (*i)++;
    return (0);
}
