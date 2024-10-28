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
                ret = process_operator(cmd->command_string, &i, cmd, &index);
            else
                ret = process_word(cmd->command_string, &i, cmd, &index);
            if (ret)
                return (1);
        }
    }
    return (0);
}

int process_operator(char *input, int *i, t_command *cmd, int *index)
{
    char *op;
    int ret;

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
    if (!is_valid_operator(op))
        cmd->token_list->wrong_operator = 1;
    return (0);
}

int process_word(char *input, int *i, t_command *cmd, int *index)
{
    char *word;
    char quote;
    int ret;

    word = ft_strdup("");
    if (!word)
        return (1);
    while (input[*i] && !ft_isspace(input[*i]) && !is_operator_char(input[*i]))
    {
        if (input[*i] == '\'' || input[*i] == '\"')
        {
            quote = input[*i];
            ret = add_char_to_token(&word, input[*i]);
            (*i)++;
            while (input[*i] && input[*i] != quote)
            {
                ret = add_char_to_token(&word, input[*i]);
                (*i)++;
            }
            if (input[*i] == quote)
            {
                ret = add_char_to_token(&word, input[*i]);
                (*i)++;
            }
        }
        else
        {
            ret = add_char_to_token(&word, input[*i]);
            (*i)++;
        }
        if (ret)
        {
            free(word);
            return (1);
        }
    }
    ret = add_token(word, &cmd->token_list, index, 0);
    if (ret)
        return (1);
    return (0);
}
