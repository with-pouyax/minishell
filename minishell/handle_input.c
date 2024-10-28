#include "minishell.h"

void handle_input(void)
{
    while (1)
    {
        g_data.input = readline(PROMPT);
        if (!g_data.input)
            break;
        if (check_unclosed_quotes(g_data.input))
        {
            char    *additional_input;

            additional_input = readline("quote> ");
            if (!additional_input)
            {
                free(g_data.input);
                break;
            }
            g_data.input = ft_strjoin_free(g_data.input, additional_input);
            if (!g_data.input)
            {
                ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
                continue;
            }
        }
        if (g_data.input && ft_strlen(g_data.input) > 0)
            add_history(g_data.input);
        g_data.commands = NULL;
        tokenize_input();
        if (!g_data.commands)
        {
            free(g_data.input);
            g_data.input = NULL;
            continue;
        }
        expand_variables_in_tokens();
        parse_tokens();
        print_commands();
        free_commands();
        g_data.commands = NULL;
        free(g_data.input);
        g_data.input = NULL;
    }
    rl_clear_history();
}
