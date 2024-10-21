#include "minishell.h"

// Function to handle user input and history
void handle_input(t_shell *shell)
{
    shell->input = readline(PROMPT);

    if (shell->input == NULL)
    {
        printf("\nexit\n");
        exit(shell->exit_status);
    }

    while (check_unclosed_quotes(shell->input))
    {
        char *additional_input = readline("quote> ");
        char *new_input = ft_strjoin(shell->input, additional_input);
        free(shell->input);
        free(additional_input);
        shell->input = new_input;
    }

    if (ft_strlen(shell->input) > 0)
        add_history(shell->input);

    lex_input(shell);
    parse_tokens(shell);

    print_commands(shell);   // For debugging

    free(shell->input);
    shell->input = NULL;
}

// Main loop that listens for input
void listen_for_input(t_shell *shell)
{
    while (1)
    {
        handle_input(shell);
    }
}
