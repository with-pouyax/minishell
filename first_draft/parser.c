#include "minishell.h"

int parse_input(char *input)
{
    t_token     *tokens;
    t_command   *cmd_list;
    int         ret;

    tokens = NULL;
    ret = tokenize_input(input, &tokens);
    if (ret != 0)
    {
        free_tokens(tokens);
        return (1);
    }
    cmd_list = parse_tokens(tokens);
    free_tokens(tokens);
    if (!cmd_list)
        return (1);
    g_shell.cmd_list = cmd_list;
    return (0);
}

t_command   *parse_tokens(t_token *tokens)
{
    t_command   *cmd_list;
    t_command   *cmd;

    cmd_list = NULL;
    while (tokens)
    {
        if (tokens->type == PIPE)
        {
            ft_putendl_fd("minishell: syntax error near unexpected token `|'", STDERR_FILENO);
            return (NULL);
        }
        cmd = create_command();
        if (!parse_command(&tokens, cmd))
        {
            free_command(cmd);
            return (NULL);
        }
        convert_args_list_to_array(cmd);
        add_command(&cmd_list, cmd);
        if (tokens && tokens->type == PIPE)
            tokens = tokens->next;
    }
    return (cmd_list);
}

int parse_command(t_token **tokens, t_command *cmd)
{
    while (*tokens && (*tokens)->type != PIPE)
    {
        if ((*tokens)->type == WORD)
        {
            if (!add_argument(cmd, (*tokens)->value))
                return (0);
        }
        else if ((*tokens)->type == INPUT_REDIRECT || (*tokens)->type == OUTPUT_REDIRECT
            || (*tokens)->type == APPEND_REDIRECT || (*tokens)->type == HEREDOC)
        {
            if (handle_redirection(tokens, cmd) != 0)
                return (0);
            continue ;
        }
        else
        {
            ft_putendl_fd("minishell: syntax error", STDERR_FILENO);
            return (0);
        }
        *tokens = (*tokens)->next;
    }
    return (1);
}

int add_argument(t_command *cmd, char *value)
{
    t_arg *new_arg;

    new_arg = malloc(sizeof(t_arg));
    if (!new_arg)
        return (0);
    new_arg->value = ft_strdup(value);
    new_arg->next = NULL;
    add_arg(&(cmd->args_list), new_arg);
    if (!cmd->cmd)
        cmd->cmd = ft_strdup(value);
    return (1);
}

void add_arg(t_arg **args_list, t_arg *new_arg)
{
    t_arg *current;

    if (!*args_list)
    {
        *args_list = new_arg;
        return ;
    }
    current = *args_list;
    while (current->next)
        current = current->next;
    current->next = new_arg;
}
