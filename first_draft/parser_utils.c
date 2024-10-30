#include "minishell.h"

void convert_args_list_to_array(t_command *cmd)
{
    int     arg_count;
    int     i;
    t_arg   *current;
    t_arg   *temp;

    arg_count = count_args(cmd->args_list);
    cmd->args = malloc(sizeof(char *) * (arg_count + 1));
    if (!cmd->args)
        return ;
    i = 0;
    current = cmd->args_list;
    while (current)
    {
        cmd->args[i++] = current->value;
        temp = current;
        current = current->next;
        free(temp);
    }
    cmd->args[i] = NULL;
    cmd->args_list = NULL;
}

int count_args(t_arg *args_list)
{
    int     count;
    t_arg   *current;

    count = 0;
    current = args_list;
    while (current)
    {
        count++;
        current = current->next;
    }
    return (count);
}

void free_args_list(t_arg *args_list)
{
    t_arg *temp;

    while (args_list)
    {
        temp = args_list;
        args_list = args_list->next;
        free(temp->value);
        free(temp);
    }
}

t_command *create_command(void)
{
    t_command *cmd;

    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);
    cmd->cmd = NULL;
    cmd->args = NULL;
    cmd->args_list = NULL;
    cmd->input_fd = STDIN_FILENO;
    cmd->output_fd = STDOUT_FILENO;
    cmd->heredoc = NULL;
    cmd->next = NULL;
    return (cmd);
}

void add_command(t_command **head, t_command *new_cmd)
{
    t_command *current;

    if (!*head)
    {
        *head = new_cmd;
        return;
    }
    current = *head;
    while (current->next)
        current = current->next;
    current->next = new_cmd;
}
