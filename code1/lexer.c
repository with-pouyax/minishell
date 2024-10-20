#include "minishell.h"

// Internal command check function
int is_internal_command(char *token)
{
    return (strcmp(token, "echo") == 0 || strcmp(token, "cd") == 0 ||
            strcmp(token, "pwd") == 0 || strcmp(token, "export") == 0 ||
            strcmp(token, "unset") == 0 || strcmp(token, "env") == 0 ||
            strcmp(token, "exit") == 0);
}

// Helper function to check if a token is an environment variable expansion
int is_variable_expansion(char *token)
{
    return (token[0] == '$');
}

// Helper function to check if a token is an operator
int is_operator(char *token)
{
    return (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0 ||
            strcmp(token, "<") == 0 || strcmp(token, "<<") == 0 ||
            strcmp(token, "|") == 0);
}

// Function to separate operators and their following tokens (like file paths)
void separate_operator(char *token, t_token **token_list, int *index)
{
    t_token *new_token = malloc(sizeof(t_token));
    new_token->value = ft_strdup(token);
    new_token->index = (*index)++;
    new_token->is_operator = 1;
    new_token->next = NULL;

    // Add the operator to the token list
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
void add_token(char *token_value, t_token **token_list, int *index)
{
    t_token *new_token = malloc(sizeof(t_token));
    new_token->value = ft_strdup(token_value);
    new_token->index = (*index)++;
    new_token->is_operator = 0;
    new_token->is_variable_expansion = is_variable_expansion(token_value);
    new_token->is_int = 0; // Default to external

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
    cmd->command_string = ft_strdup(shell->input); // Store raw command string
    cmd->index = 0;
    cmd->next = NULL;
    cmd->token_list = NULL;
    
    shell->commands = cmd;
    tokenize_command(cmd);
}

// Function to tokenize a single command string
void tokenize_command(t_command *command)
{
    char **tokens = ft_split(command->command_string, ' ');
    t_token *token_list = NULL;
    int index = 0;

    for (int i = 0; tokens[i] != NULL; i++)
    {
        if (is_operator(tokens[i]))
        {
            // Separate the operator and any following token (like file path)
            separate_operator(tokens[i], &token_list, &index);
        }
        else
        {
            add_token(tokens[i], &token_list, &index);

            if (index == 1) // If it's the first token (i.e., the command)
            {
                token_list->is_command = 1;
                if (is_internal_command(tokens[i]))
                    token_list->is_int = 1; // Mark as internal if it's a built-in
            }
        }
    }

    // Link token list to the command
    command->token_list = token_list;
    free(tokens);
}

// Helper function to check for unclosed quotes
int check_unclosed_quotes(char *input)
{
    int in_single_quote = 0;
    int in_double_quote = 0;

    while (*input)
    {
        if (*input == '\'' && !in_double_quote) // Toggle single quote flag
            in_single_quote = !in_single_quote;
        else if (*input == '"' && !in_single_quote) // Toggle double quote flag
            in_double_quote = !in_double_quote;
        
        input++;
    }

    // If either flag is still active, it means there's an unclosed quote
    return (in_single_quote || in_double_quote);
}
