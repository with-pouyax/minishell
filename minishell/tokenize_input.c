#include "minishell.h"

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

int tokenize_command(t_command *cmd)
{
    int ret;

    g_data.i = 0;
    g_data.token_index = 0;
    while (cmd->command_string[g_data.i])
    {
        while (cmd->command_string[g_data.i] && ft_isspace(cmd->command_string[g_data.i]))
            g_data.i++;
        if (cmd->command_string[g_data.i])
        {
            if (is_operator_char(cmd->command_string[g_data.i]))
                ret = process_operator(cmd->command_string, cmd);
            else
                ret = process_word(cmd->command_string, cmd);
            if (ret)
            {
                free_tokens(cmd->token_list);
                cmd->token_list = NULL;
                return (1);
            }
        }
    }
    return (0);
}



int process_operator(char *input, t_command *cmd)
{
    char *op;
    int ret;
    t_token *last_token;

    op = ft_strdup("");
    if (!op)
        return (1);
    while (input[g_data.i] && is_operator_char(input[g_data.i]))
    {
        ret = add_char_to_token(&op, input[g_data.i]);
        if (ret)
        {
            free(op);
            return (1);
        }
        g_data.i++;
    }
    ret = add_token(op, &cmd->token_list, 1);
    if (ret)
    {
        free(op);
        return (1);
    }
    if (!is_valid_operator(op))
        cmd->token_list->wrong_operator = 1;

    last_token = cmd->token_list;
    while (last_token->next)
        last_token = last_token->next;

    if (ft_strcmp(op, "<<") == 0)
    {
        last_token->is_heredoc = 1;
        ret = process_heredoc_delimiter(input, last_token);
        if (ret)
            return (1);
    }
    else
    {
        g_data.token_index++;
    }
    return (0);
}





int process_word(char *input, t_command *cmd)
{
    char *word;
    char quote;
    int ret;

    word = ft_strdup("");
    if (!word)
        return (1);
    while (input[g_data.i] && !ft_isspace(input[g_data.i]) && !is_operator_char(input[g_data.i]))
    {
        if (input[g_data.i] == '\'' || input[g_data.i] == '\"')
        {
            quote = input[g_data.i];
            ret = add_char_to_token(&word, input[g_data.i]);
            g_data.i++;
            while (input[g_data.i] && input[g_data.i] != quote)
            {
                ret = add_char_to_token(&word, input[g_data.i]);
                g_data.i++;
            }
            if (input[g_data.i] == quote)
            {
                ret = add_char_to_token(&word, input[g_data.i]);
                g_data.i++;
            }
        }
        else
        {
            ret = add_char_to_token(&word, input[g_data.i]);
            g_data.i++;
        }
        if (ret)
        {
            free(word);
            return (1);
        }
    }
    ret = add_token(word, &cmd->token_list, 0);
    if (ret)
    {
        free(word);
        return (1);
    }
    return (0);
}
