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

    g_data.input = ft_strjoin_and_free_first(g_data.input, additional_input);
    if (!g_data.input)
    {
        ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
        free(additional_input);
        return (-1);
    }

    // Append the prompt and additional input to full_input
    g_data.full_input = ft_strjoin_and_free_first(g_data.full_input, "\nquote> ");
    if (!g_data.full_input)
    {
        ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
        return (-1);
    }

    g_data.full_input = ft_strjoin_and_free_first(g_data.full_input, additional_input);
    free(additional_input); // Free after use
    if (!g_data.full_input)
    {
        ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
        return (-1);
    }

    return (1);
}





void process_input(void)
{
    // Use full_input for history if it contains heredoc content
    if (g_data.full_input && ft_strlen(g_data.full_input) > 0)
        add_history(g_data.full_input);
    else if (g_data.input && ft_strlen(g_data.input) > 0)
        add_history(g_data.input);

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

        g_data.full_input = ft_strdup(g_data.input);

        if (check_unclosed_quotes(g_data.input))
        {
            ret = handle_unclosed_quotes();
            if (ret <= 0)
            {
                cleanup();
                break;
            }
        }

        process_input();

        // Free both input and full_input after processing
        free(g_data.input);
        g_data.input = NULL;
        free(g_data.full_input);
        g_data.full_input = NULL;
    }
    rl_clear_history();
}




