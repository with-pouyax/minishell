#include "minishell.h"

int handle_redirection(t_token **tokens, t_command *cmd)
{
    t_token *token;
    int     fd;

    token = *tokens;
    if (!token->next || token->next->type != WORD)
    {
        ft_putendl_fd("minishell: syntax error near unexpected token", STDERR_FILENO);
        return (1);
    }
    token = token->next;
    if ((*tokens)->type == INPUT_REDIRECT)
    {
        fd = open(token->value, O_RDONLY);
        if (fd < 0)
            return (print_error(token->value));
        if (cmd->input_fd != STDIN_FILENO)
            close(cmd->input_fd);
        cmd->input_fd = fd;
    }
    else if ((*tokens)->type == OUTPUT_REDIRECT)
    {
        fd = open(token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
            return (print_error(token->value));
        if (cmd->output_fd != STDOUT_FILENO)
            close(cmd->output_fd);
        cmd->output_fd = fd;
    }
    else if ((*tokens)->type == APPEND_REDIRECT)
    {
        fd = open(token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0)
            return (print_error(token->value));
        if (cmd->output_fd != STDOUT_FILENO)
            close(cmd->output_fd);
        cmd->output_fd = fd;
    }
    else if ((*tokens)->type == HEREDOC)
    {
        if (handle_heredoc(token->value, cmd) != 0)
            return (1);
    }
    *tokens = token->next;
    return (0);
}

void    free_tokens(t_token *tokens)
{
    t_token *temp;

    while (tokens)
    {
        temp = tokens;
        tokens = tokens->next;
        free(temp->value);
        free(temp);
    }
}

int print_error(char *message)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    perror(message);
    return (1);
}

void    free_command(t_command *cmd)
{
    if (cmd->cmd)
        free(cmd->cmd);
    if (cmd->args)
        free_array(cmd->args);
    if (cmd->input_fd != STDIN_FILENO)
        close(cmd->input_fd);
    if (cmd->output_fd != STDOUT_FILENO)
        close(cmd->output_fd);
    free(cmd);
}

void    free_command_list(void)
{
    t_command   *cmd_list;
    t_command   *temp;

    cmd_list = g_shell.cmd_list;
    while (cmd_list)
    {
        temp = cmd_list;
        cmd_list = cmd_list->next;
        free_command(temp);
    }
    g_shell.cmd_list = NULL;
}
