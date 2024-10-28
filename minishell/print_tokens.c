#include "minishell.h"

void print_token_details(t_token *token)
{
    if (token->is_operator)
    {
        printf(" - This is an operator.\n");
        if (token->wrong_operator)
            printf(" - Warning: Invalid operator detected.\n");
    }
    else if (token->is_command)
    {
        printf(" - This is a command.\n");
        if (token->is_int)
            printf(" - This is an internal command.\n");
        else
            printf(" - This is an external command.\n");
    }
    if (token->is_flag)
        printf(" - This is a flag.\n");
}

void print_tokens(t_token *token_list)
{
    t_token *token;

    token = token_list;
    while (token)
    {
        printf("Token: '%s', Index: %d\n", token->value, token->index);
        print_token_details(token);
        token = token->next;
    }
}

void print_commands(void)
{
    t_command *cmd;

    cmd = g_data.commands;
    while (cmd)
    {
        printf("Command #%d: '%s'\n", cmd->index, cmd->command_string);
        print_tokens(cmd->token_list);
        cmd = cmd->next;
    }
}
