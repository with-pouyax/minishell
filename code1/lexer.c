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

// Tokenize the entire input into commands and tokens
void tokenize_input(char *input, t_shell *shell)
{
    int i;
    int start;
    t_command *cmd;
    t_command *last_cmd;
    char *cmd_str;
    int cmd_index;

    i = 0;
    cmd_index = 0;
    last_cmd = NULL;
    while (input[i])
    {
        while (input[i] && ft_isspace(input[i]))
            i++; // Skip whitespaces
        start = i;
        // Find the next part of the input (either a command or a pipe)
        if (input[i] == '|')
        {
            // Pipe operator detected, add it as a separate command node
            cmd_str = ft_strdup("|");
            i++;
        }
        else
        {
            // Process the command (until we reach a pipe or the end)
            while (input[i] && input[i] != '|' && !ft_isspace(input[i]))
            {
                if (input[i] == '\'' || input[i] == '\"')
                    i = skip_quotes(input, i); // Skip over quotes
                else
                    i++;
            }
            cmd_str = ft_substr(input, start, i - start); // Extract the command part
        }

        // Create a new command node
        cmd = create_command(cmd_str, cmd_index++);
        tokenize_command(cmd); // Tokenize the command into tokens

        // Add the command to the linked list
        if (!shell->commands)
            shell->commands = cmd;
        else
            last_cmd->next = cmd;
        last_cmd = cmd;
    }
}

// Create a new command node
t_command *create_command(char *cmd_str, int index)
{
    t_command *cmd;

    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);
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

    op = ft_strdup("");  // Allocate memory for the operator string
    while (input[*i] && is_operator_char(input[*i]))  // Check if it's an operator character
    {
        add_char_to_token(&op, input[*i]);  // Add the operator character to the token
        (*i)++;  // Move to the next character
    }
    add_token(op, &cmd->token_list, index, 1);  // Add the operator token to the token list
    if (!is_valid_operator(op))  // Check if it's a valid operator
    {
        t_token *new_token = cmd->token_list;  // Traverse to the newly added token
        while (new_token->next)
            new_token = new_token->next;
        new_token->wrong_operator = 1;  // Mark the token as a wrong operator if necessary
    }
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

    quote = input[i++];
    while (input[i] && input[i] != quote)
        i++;
    if (input[i] == quote)
        i++;
    return (i);
}

// Add character to token
void add_char_to_token(char **token, char c)
{
    char tmp[2];

    tmp[0] = c;
    tmp[1] = '\0';
    *token = ft_strjoin_free(*token, ft_strdup(tmp));
}
