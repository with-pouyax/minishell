#include "minishell.h"

// Function to separate operators
void separate_operator(char *token, t_token **token_list, int *index)
{
    t_token *new_token = malloc(sizeof(t_token));

    new_token->value = ft_strdup(token);
    new_token->index = (*index)++;
    new_token->is_operator = 1;
    new_token->next = NULL;

    if (!*token_list)
        *token_list = new_token;
    else
    {
        t_token *current = *token_list;
        while (current->next)
            current = current->next;
        current->next = new_token;
    }
}

// Function to add a new token to the token list
void add_token(char *token_value, t_token **token_list, int *index, t_shell *shell)
{
    t_token *new_token = malloc(sizeof(t_token));
    char    *expanded_value;
    int     var_not_found_flag = 0;

    expanded_value = expand_variables(token_value, shell, &var_not_found_flag);
    new_token->value = expanded_value;
    new_token->index = (*index)++;
    new_token->is_operator = 0;
    new_token->is_int = 0;
    new_token->var_not_found = var_not_found_flag; // Set the flag in the token

    new_token->next = NULL;

    if (!*token_list)
        *token_list = new_token;
    else
    {
        t_token *current = *token_list;
        while (current->next)
            current = current->next;
        current->next = new_token;
    }
}


// Function to lex the input by splitting it into commands and tokens
void lex_input(t_shell *shell)
{
    t_command *cmd = malloc(sizeof(t_command));
    cmd->command_string = ft_strdup(shell->input);
    cmd->index = 0;
    cmd->next = NULL;
    cmd->token_list = NULL;

    shell->commands = cmd;
    tokenize_command(cmd, shell);
}

// Function to tokenize a single command string
void tokenize_command(t_command *command, t_shell *shell)
{
    char **tokens = ft_split(command->command_string, ' ');
    t_token *token_list = NULL;
    int index = 0;

    for (int i = 0; tokens[i] != NULL; i++)
    {
        if (is_operator(tokens[i]))
        {
            separate_operator(tokens[i], &token_list, &index);
        }
        else
        {
            add_token(tokens[i], &token_list, &index, shell);

            if (index == 1)
            {
                token_list->is_command = 1;
                if (is_internal_command(token_list->value))
                    token_list->is_int = 1;
            }
        }
    }

    command->token_list = token_list;
    // Free tokens array
    for (int i = 0; tokens[i]; i++)
        free(tokens[i]);
    free(tokens);
}
