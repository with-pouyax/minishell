#include "minishell.h"

void parse_tokens(t_shell_data *shell)
{
    t_command *cmd;
    t_token *token;

    cmd = shell->commands;
    while (cmd) // Loop through all commands
    {
        token = cmd->token_list;
        while (token) // Loop through all tokens in the command
        {
            if (token->index == 0 && !token->is_operator) // If the token is the first token and not an operator
            {
                token->is_command = 1;
                if (is_internal_command(token->value)) // If the token is an internal command
                    token->is_int = 1; // Set the is_int flag to 1
            }
            else if (ft_strncmp(token->value, "-", 1) == 0) // If the token starts with a dash
                token->is_flag = 1; // Set the is_flag flag to 1
            token = token->next; // Move to the next token
        }
        cmd = cmd->next; // Move to the next command
    }
}

