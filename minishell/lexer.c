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
void tokenize_input(void)
{
    int         i;
    int         start;
    t_command   *cmd;
    t_command   *last_cmd;
    char        *cmd_str;
    int         cmd_index;

    i = 0;
    cmd_index = 0;
    last_cmd = NULL;
    g_data.error_flag = 0;
    while (g_data.input[i])
    {
        while (g_data.input[i] && ft_isspace(g_data.input[i]))
            i++;
        start = i;
        if (g_data.input[i] == '|')
        {
            cmd_str = ft_strdup("|");
            if (!cmd_str)
            {
                g_data.error_flag = 2;
                break;
            }
            i++;
        }
        else
        {
            while (g_data.input[i] && g_data.input[i] != '|' && !ft_isspace(g_data.input[i]))
            {
                if (g_data.input[i] == '\'' || g_data.input[i] == '\"')
                    i = skip_quotes(g_data.input, i);
                else
                    i++;
            }
            cmd_str = ft_substr(g_data.input, start, i - start);
            if (!cmd_str)
            {
                g_data.error_flag = 2;
                break;
            }
        }
        cmd = create_command(cmd_str, cmd_index++);
        if (!cmd)
        {
            free(cmd_str);
            g_data.error_flag = 2;
            break;
        }
        if (tokenize_command(cmd))
        {
            free(cmd_str);
            free(cmd);
            g_data.error_flag = 2;
            break;
        }
        if (!g_data.commands)
            g_data.commands = cmd;
        else
            last_cmd->next = cmd;
        last_cmd = cmd;
    }
    if (g_data.error_flag)
    {
        free_commands();
        g_data.commands = NULL;
        if (g_data.error_flag == 1)
        {
            ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
            g_data.exit_status = 2;
        }
        else if (g_data.error_flag == 2)
        {
            ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
            exit(EXIT_FAILURE);
        }
    }
}






// Create a new command node
t_command   *create_command(char *cmd_str, int index)
{
    t_command   *cmd;

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
int tokenize_command(t_command *cmd)
{
    int i;
    int index;
    int ret;

    i = 0;
    index = 0;
    while (cmd->command_string[i])
    {
        while (cmd->command_string[i] && ft_isspace(cmd->command_string[i]))
            i++;
        if (cmd->command_string[i])
        {
            if (is_operator_char(cmd->command_string[i]))
            {
                ret = process_operator(cmd->command_string, &i, cmd, &index);
                if (ret)
                    return (1);
            }
            else
            {
                ret = process_word(cmd->command_string, &i, cmd, &index);
                if (ret)
                    return (1);
            }
        }
    }
    return (0);
}

// Check if character is an operator character
int is_operator_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}


// Process operator tokens
int process_operator(char *input, int *i, t_command *cmd, int *index)
{
    char    *op;
    int     ret;
    t_token *new_token;

    op = ft_strdup("");
    if (!op)
        return (1);
    while (input[*i] && is_operator_char(input[*i]))
    {
        ret = add_char_to_token(&op, input[*i]);
        if (ret)
        {
            free(op);
            return (1);
        }
        (*i)++;
    }
    ret = add_token(op, &cmd->token_list, index, 1);
    if (ret)
    {
        free(op);
        return (1);
    }
    new_token = cmd->token_list;
    while (new_token->next)
        new_token = new_token->next;
    if (!is_valid_operator(op))
        new_token->wrong_operator = 1;
    return (0);
}





// Check if operator is valid
int is_valid_operator(char *op)
{
    return (!ft_strcmp(op, "|") || !ft_strcmp(op, "<") || !ft_strcmp(op, ">") ||
            !ft_strcmp(op, ">>") || !ft_strcmp(op, "<<"));
}

// Process word tokens
int process_word(char *input, int *i, t_command *cmd, int *index)
{
    char    *word;
    char    quote;
    int     ret;

    word = ft_strdup("");
    if (!word)
        return (1);
    while (input[*i] && !ft_isspace(input[*i]) && !is_operator_char(input[*i]))
    {
        if (input[*i] == '\'' || input[*i] == '\"')
        {
            quote = input[*i];
            ret = add_char_to_token(&word, input[*i]);
            if (ret)
            {
                free(word);
                return (1);
            }
            (*i)++;
            while (input[*i] && input[*i] != quote)
            {
                ret = add_char_to_token(&word, input[*i]);
                if (ret)
                {
                    free(word);
                    return (1);
                }
                (*i)++;
            }
            if (input[*i] == quote)
            {
                ret = add_char_to_token(&word, input[*i]);
                if (ret)
                {
                    free(word);
                    return (1);
                }
                (*i)++;
            }
        }
        else
        {
            ret = add_char_to_token(&word, input[*i]);
            if (ret)
            {
                free(word);
                return (1);
            }
            (*i)++;
        }
    }
    ret = add_token(word, &cmd->token_list, index, 0);
    if (ret)
        return (1);
    return (0);
}



// Skip over quoted strings
int skip_quotes(char *input, int i)
{
    char    quote;

    quote = input[i++];
    while (input[i] && input[i] != quote)
        i++;
    if (input[i] == quote)
        i++;
    return (i);
}

// Add character to token
int add_char_to_token(char **token, char c)
{
    char    tmp[2];
    char    *new_token;

    tmp[0] = c;
    tmp[1] = '\0';
    new_token = ft_strjoin_safe(*token, tmp);
    if (!new_token)
    {
        free(*token);
        *token = NULL;
        return (1);
    }
    free(*token);
    *token = new_token;
    return (0);
}
