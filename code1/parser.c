#include "minishell.h"

// Parse tokens and identify their types
void parse_tokens(t_shell *shell)
{
    t_command *cmd = shell->commands;

    while (cmd)
    {
        t_token *token = cmd->token_list;
        while (token)
        {
            if (token->index == 0)
                token->is_command = 1;

            if (ft_strncmp(token->value, "-", 1) == 0)
                token->is_flag = 1;

            token = token->next;
        }
        cmd = cmd->next;
    }
}
