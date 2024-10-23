/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyaximac <pouyaximac@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by yourname          #+#    #+#             */
/*   Updated: 2024/10/21 21:02:10 by pouyaximac       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int is_valid_pipe(char *input, int i)
{
    int prev_char_index = i - 1;
    int next_char_index = i + 1;
    int input_len = ft_strlen(input);

    // Check for multiple '|' or operators after '|'
    if (input[i + 1] == '|' || input[i + 1] == '>' || input[i + 1] == '<')
        return 0;

    // Check for at least one whitespace before '|'
    if (prev_char_index < 0 || !ft_isspace(input[prev_char_index]))
        return 0;

    // Skip backward over whitespace
    while (prev_char_index >= 0 && ft_isspace(input[prev_char_index]))
        prev_char_index--;

    // Now prev_char_index is at the last non-space character before the pipe
    // Check that it's not an operator character
    if (prev_char_index < 0 || is_operator_char(input[prev_char_index]))
        return 0;

    // Check for at least one whitespace after '|'
    if (next_char_index >= input_len || !ft_isspace(input[next_char_index]))
        return 0;

    // Skip forward over whitespace
    while (next_char_index < input_len && ft_isspace(input[next_char_index]))
        next_char_index++;

    // Now next_char_index is at the first non-space character after the pipe
    // Check that it's not an operator character
    if (next_char_index >= input_len || is_operator_char(input[next_char_index]))
        return 0;

    return 1;
}





// Tokenize the entire input into commands and tokens
void tokenize_input(char *input, t_shell *shell)
{
    int i;
    int start;
    t_command *cmd;
    t_command *last_cmd;
    char *cmd_str;
    int cmd_index;
    int error_flag = 0;

    i = 0;
    cmd_index = 0;
    last_cmd = NULL;
    while (input[i]) // Loop through the input string
    {
        while (input[i] && ft_isspace(input[i])) // Skip whitespaces
            i++; 
        start = i; // set index of first character we find to start

        if (input[i] == '|') // If we find a pipe
        {
            if (is_valid_pipe(input, i)) // if pipe is valid
            {
                cmd_str = ft_strdup("|"); // we create a new node with the pipe in it
                i++; // move to the next character
            }
            else // if pipe is not valid
            {
                error_flag = 1; // set error flag to 1
                break; // break the loop
            }
        } 
        else // If we find a command (not a pipe)
        {
            while (input[i] && input[i] != '|' && !ft_isspace(input[i])) // Loop until we find a pipe or whitespace
            {
                if (input[i] == '\'' || input[i] == '\"') // If we find a quote
                    i = skip_quotes(input, i); // Skip over the quoted string and move to the next character
                else // If we find a regular character
                    i++; // Move to the next character
            }
            // after we check for quotes and regular characters, we create a new node with the command string
            cmd_str = ft_substr(input, start, i - start); // Create a substring from the start index to the current index and store it in cmd_str
        }

        cmd = create_command(cmd_str, cmd_index++); // we create a new node with the command string and index
        tokenize_command(cmd); // Tokenize the command into tokens

        // Add the command to the linked list
        if (!shell->commands)
            shell->commands = cmd;
        else
            last_cmd->next = cmd;
        last_cmd = cmd;
    }

    if (error_flag)
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
        shell->exit_status = 2;
        free_commands(shell->commands);
        shell->commands = NULL;
        return;
    }
}



// Create a new command node
t_command *create_command(char *cmd_str, int index)
{
    t_command *cmd;

    cmd = malloc(sizeof(t_command)); // Allocate memory for the new node
    if (!cmd)
    {
        printf("Error: malloc failed\n");
        free(cmd_str);
        exit(EXIT_FAILURE);
    }
    cmd->command_string = cmd_str;
    cmd->index = index;
    cmd->token_list = NULL;
    cmd->next = NULL;
    return (cmd);
}

// Tokenize a single command string into tokens
void tokenize_command(t_command *cmd)
{
    int     i;
    int     index;

    i = 0;
    index = 0;
    while (cmd->command_string[i])
    {
        while (cmd->command_string[i] && ft_isspace(cmd->command_string[i]))
            i++;
        if (cmd->command_string[i])
        {
            if (is_operator_char(cmd->command_string[i]))
                process_operator(cmd->command_string, &i, cmd, &index);
            else
                process_word(cmd->command_string, &i, cmd, &index);
        }
    }
}

// Check if character is an operator character
int is_operator_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}


// Process operator tokens
void process_operator(char *input, int *i, t_command *cmd, int *index)
{
    char *op;

    op = ft_strdup("");
    while (input[*i] && is_operator_char(input[*i]))
    {
        add_char_to_token(&op, input[*i]);
        (*i)++;
    }
    add_token(op, &cmd->token_list, index, 1);

    // Set wrong_operator flag if the operator is invalid
    t_token *new_token = cmd->token_list;
    while (new_token->next)
        new_token = new_token->next;
    if (!is_valid_operator(op))
        new_token->wrong_operator = 1;
}


// Check if operator is valid
int is_valid_operator(char *op)
{
    return (!ft_strcmp(op, "|") || !ft_strcmp(op, "<") || !ft_strcmp(op, ">") ||
            !ft_strcmp(op, ">>") || !ft_strcmp(op, "<<"));
}

// Process word tokens
void process_word(char *input, int *i, t_command *cmd, int *index)
{
    char    *word;
    char    quote;

    word = ft_strdup("");
    while (input[*i] && !ft_isspace(input[*i]) && !is_operator_char(input[*i]))
    {
        if (input[*i] == '\'' || input[*i] == '\"')
        {
            quote = input[*i];
            add_char_to_token(&word, input[*i]); // Include the opening quote
            (*i)++;
            while (input[*i] && input[*i] != quote)
            {
                add_char_to_token(&word, input[*i]);
                (*i)++;
            }
            if (input[*i] == quote)
            {
                add_char_to_token(&word, input[*i]); // Include the closing quote
                (*i)++;
            }
        }
        else
        {
            add_char_to_token(&word, input[*i]);
            (*i)++;
        }
    }
    add_token(word, &cmd->token_list, index, 0);
}

// Skip over quoted strings
int skip_quotes(char *input, int i)
{
    char quote;

    quote = input[i++]; // store the opening quote in quote and move to the next character
    while (input[i] && input[i] != quote) // Skip all characters until the closing quote is found
        i++; 
    if (input[i] == quote) // If the closing quote is found 
        i++; // Move to the next character
    return (i); // Return the index of the next character
}

// Add character to token
void add_char_to_token(char **token, char c)
{
    char tmp[2];

    tmp[0] = c;
    tmp[1] = '\0';
    *token = ft_strjoin_free(*token, ft_strdup(tmp));
}
