#include "minishell.h"

int add_token(char *token_value, t_token **token_list, int is_operator)
{
    t_token *new_token;
    t_token *current;

    new_token = malloc(sizeof(t_token));
    if (!new_token)
    {
        // Do not free token_value here; let the caller handle it
        return (1);
    }
    new_token->value = token_value;
    new_token->index = g_data.token_index++;
    new_token->is_operator = is_operator;
    new_token->is_command = 0;
    new_token->is_flag = 0;
    new_token->is_int = 0;
    new_token->var_not_found = 0;
    new_token->wrong_operator = 0;
    new_token->is_heredoc = 0;
    new_token->heredoc_file = NULL;
    new_token->heredoc_delimiter = NULL;
    new_token->next = NULL;

    if (!*token_list)
        *token_list = new_token;
    else
    {
        current = *token_list;
        while (current->next)
            current = current->next;
        current->next = new_token;
    }
    return (0);
}
