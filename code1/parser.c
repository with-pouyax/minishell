#include "minishell.h"

// Parse tokens and identify their types (command, flag, argument, etc.)
void parse_tokens(t_shell *shell)
{
    t_command *cmd = shell->commands;

    while (cmd)
    {
        t_token *token = cmd->token_list;
        while (token)
        {
            if (token->index == 0)  // First token is usually the command
                token->is_command = 1;

            // Detect flags and other types of tokens
            if (ft_strncmp(token->value, "-", 1) == 0)
                token->is_flag = 1;

            token = token->next;
        }
        cmd = cmd->next;
    }
}
