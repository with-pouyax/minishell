#include "internal_commands.h"

/*
shell->exit_status = ret
----> Set global exit status based on command return value
*/
static int	execute_command(t_shell_data *shell, t_command *cmd,
				t_token *token, int ret)
{
	if (!shell || !cmd || !token)
        return (-1);
	if (ft_strcmp(token->value, "echo") == 0)
		ret = ft_echo(shell, cmd);
	else if (ft_strcmp(token->value, "cd") == 0)
		ret = ft_cd(shell, cmd);
	else if (ft_strcmp(token->value, "pwd") == 0)
		ret = ft_pwd(shell, cmd);
	else if (ft_strcmp(token->value, "export") == 0)
		ret = ft_export(shell, cmd);
	else if (ft_strcmp(token->value, "unset") == 0)
		ret = ft_unset(shell, cmd);
	else if (ft_strcmp(token->value, "env") == 0)
		ret = ft_env(shell, cmd);
	else if (ft_strcmp(token->value, "exit") == 0)
		ret = ft_exit_shell(shell, cmd);
	else
	{
		write_error(token->value, ": command not found\n");
		shell->exit_status = 127;
		return (-1);
	}
	// printf("DEBUG: Command %s returned %d\n", token->value, ret);
	shell->exit_status = ret;
	return (ret);
}

int execute_internal_commands(t_shell_data *shell, t_command *cmds)
{
    t_token *token;
	int		ret;

    if (!cmds || !cmds->token_list)
    {
		return (0);
	}
    token = cmds->token_list;
    while (token)
    {
        if (token->is_command && token->is_int)
        {
            ret = execute_command(shell, cmds, token, 0);
            if (ret == -1)
				return (-1);
        }
        // if (token->is_end)
        //     break;
        token = token->next;
    }
    return (0);
}
