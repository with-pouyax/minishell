#include "minishell.h"

void cleanup(void)
{
    free(g_data.input);
    g_data.input = NULL;
    free(g_data.full_input);
    g_data.full_input = NULL;
    free_commands();
}

void handle_tokenization_error(int error_flag)
{
    if (error_flag == 1)
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
                     STDERR_FILENO);
        g_data.exit_status = 2;
    }
    else if (error_flag == 2)
    {
        ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
        cleanup();
        exit(EXIT_FAILURE);
    }
}


void tokenize_input(void)
{
    int start;
    t_command *cmd;
    t_command *last_cmd;
    char *cmd_str;
    int cmd_index;

    g_data.i = 0;
    cmd_index = 0;
    last_cmd = NULL;
    g_data.error_flag = 0;
    while (g_data.input[g_data.i])
    {
        while (g_data.input[g_data.i] && ft_isspace(g_data.input[g_data.i]))
            g_data.i++;
        start = g_data.i;
        while (g_data.input[g_data.i] && g_data.input[g_data.i] != '|')
        {
            if (g_data.input[g_data.i] == '\'' || g_data.input[g_data.i] == '\"')
                g_data.i = skip_quotes(g_data.input, g_data.i);
            else
                g_data.i++;
        }
        cmd_str = ft_substr(g_data.input, start, g_data.i - start);
        if (!cmd_str)
        {
            g_data.error_flag = 2;
            break;
        }
        cmd = create_command(cmd_str, cmd_index++);
        if (!cmd || tokenize_command(cmd))
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
        if (g_data.input[g_data.i] == '|')
            g_data.i++;
    }
    if (g_data.error_flag)
    {
        free_commands();
        g_data.commands = NULL;
        handle_tokenization_error(g_data.error_flag);
    }
}


