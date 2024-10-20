#include "minishell.h"

// Free the token list
void free_tokens(t_token *token_list)
{
    t_token *tmp;
    
    while (token_list)
    {
        tmp = token_list;
        token_list = token_list->next;
        free(tmp->value);  // Free the token value string
        free(tmp);  // Free the token structure
    }
}

// Free the command list and its associated tokens
void free_commands(t_command *cmd_list)
{
    t_command *tmp;
    
    while (cmd_list)
    {
        tmp = cmd_list;
        free_tokens(cmd_list->token_list);  // Free all tokens in the command
        free(cmd_list->command_string);  // Free the raw command string
        cmd_list = cmd_list->next;
        free(tmp);  // Free the command structure
    }
}
