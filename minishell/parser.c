#include "minishell.h"

int	is_internal_command(char *token)
{
	if (!ft_strcmp(token, "echo") || !ft_strcmp(token, "cd")
		|| !ft_strcmp(token, "pwd") || !ft_strcmp(token, "export")
		|| !ft_strcmp(token, "unset") || !ft_strcmp(token, "env")
		|| !ft_strcmp(token, "exit"))
		return (1);
	return (0);
}

void parse_tokens(void)
{
    t_command *cmd;
    t_token *token;

    cmd = g_data.commands;
    while (cmd)
    {
        token = cmd->token_list;
        while (token)
        {
            // Do not reclassify tokens based on their value
            if (token->index == 0 && !token->is_operator)
            {
                token->is_command = 1;
                if (is_internal_command(token->value))
                    token->is_int = 1;
            }
            else if (ft_strncmp(token->value, "-", 1) == 0)
                token->is_flag = 1;
            token = token->next;
        }
        cmd = cmd->next;
    }
}

