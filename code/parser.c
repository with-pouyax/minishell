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
    int         arg_count;

    cmd_list = NULL;
    while (tokens)
    {
        if (tokens->type == PIPE)
        {
            ft_putendl_fd("minishell: syntax error near unexpected token `|'", STDERR_FILENO);
            return (NULL);
        }
        cmd = create_command();
        arg_count = 0;
        while (tokens && tokens->type != PIPE)
        {
            if (tokens->type == WORD)
            {
                cmd->args = realloc(cmd->args, sizeof(char *) * (arg_count + 2));
                cmd->args[arg_count++] = ft_strdup(tokens->value);
                cmd->args[arg_count] = NULL;
                if (!cmd->cmd)
                    cmd->cmd = ft_strdup(tokens->value);
            }
            else if (tokens->type == INPUT_REDIRECT || tokens->type == OUTPUT_REDIRECT
                || tokens->type == APPEND_REDIRECT || tokens->type == HEREDOC)
            {
                if (handle_redirection(&tokens, cmd) != 0)
                {
                    free_command(cmd);
                    return (NULL);
                }
                continue ;
            }
            else
            {
                ft_putendl_fd("minishell: syntax error", STDERR_FILENO);
                free_command(cmd);
                return (NULL);
            }
            tokens = tokens->next;
        }
        add_command(&cmd_list, cmd);
        if (tokens)
            tokens = tokens->next;
    }
    return (cmd_list);
}

t_command   *create_command(void)
{
    t_command   *cmd;

    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);
    cmd->cmd = NULL;
    cmd->args = NULL;
    cmd->input_fd = STDIN_FILENO;
    cmd->output_fd = STDOUT_FILENO;
    cmd->heredoc = NULL;
    cmd->next = NULL;
    return (cmd);
}

void    add_command(t_command **head, t_command *new_cmd)
{
    t_command   *current;

    if (!*head)
    {
        *head = new_cmd;
        return ;
    }
    current = *head;
    while (current->next)
        current = current->next;
    current->next = new_cmd;
}
