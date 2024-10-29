#include "minishell.h"

int handle_unclosed_quotes(void)
{
    char *additional_input;

    additional_input = readline("quote> ");
    if (!additional_input)
    {
        free(g_data.input);
        return (0);
    }
    g_data.input = ft_strjoin_free(g_data.input, additional_input);
    if (!g_data.input)
    {
        ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
        return (-1);
    }
    return (1);
}

void process_input(void)
{
    if (g_data.input && ft_strlen(g_data.input) > 0)
        add_history(g_data.input);

    // Call preprocess_input() here
    preprocess_input();

    g_data.commands = NULL;
    tokenize_input();
    if (!g_data.commands)
        return;
    expand_variables_in_tokens();
    parse_tokens();
    print_commands();
    free_commands();
}


void handle_input(void)
{
    int ret;

    while (1)
    {
        g_data.input = readline(PROMPT);
        if (!g_data.input)
            break;
        if (check_unclosed_quotes(g_data.input))
        {
            ret = handle_unclosed_quotes();
            if (ret <= 0)
                break;
        }
        process_input();
        free(g_data.input);
        g_data.input = NULL;
    }
    rl_clear_history();
}
