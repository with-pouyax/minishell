#include "minishell.h"

void free_tokens(t_token *token_list)
{
    t_token *token;
    t_token *next_token;

    token = token_list;
    while (token)
    {
        next_token = token->next;
        free(token->value);
        free(token);
        token = next_token;
    }
}

void free_commands(void)
{
    t_command *cmd;
    t_command *next_cmd;

    cmd = g_data.commands;
    while (cmd)
    {
        next_cmd = cmd->next;
        free_tokens(cmd->token_list);
        free(cmd->command_string);
        free(cmd);
        cmd = next_cmd;
    }
    g_data.commands = NULL;
}
