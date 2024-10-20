#include "minishell.h"

// Initialize shell state
void init_shell(t_shell *shell)
{
    shell->input = NULL;
    shell->exit_status = 0;
    shell->commands = NULL;  // Initialize commands list
}

// Function to handle user input and history
void handle_input(t_shell *shell)
{
    shell->input = readline(PROMPT);

    // Check for EOF (Ctrl+D) or empty input
    if (shell->input == NULL)
    {
        printf("\nexit\n");
        exit(shell->exit_status);
    }

    // Check for unclosed quotes and keep prompting until they're closed
    while (check_unclosed_quotes(shell->input))
    {
        char *additional_input = readline("quote> ");
        char *new_input = ft_strjoin(shell->input, additional_input);
        free(shell->input);
        free(additional_input);
        shell->input = new_input;
    }

    // Add input to history if it's not empty
    if (strlen(shell->input) > 0)
        add_history(shell->input);

    // Lexing and parsing input
    lex_input(shell);        // Break the input into tokens
    parse_tokens(shell);     // Process tokens for execution

    // Print the command and token structure for verification
    print_commands(shell);   // TEMPORARY: Print commands and tokens for debugging

    // Cleanup
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

// Cleanup shell state
void cleanup_shell(t_shell *shell)
{
    if (shell->input)
        free(shell->input);
}

// Main function
int main(void)
{
    t_shell shell;

    init_shell(&shell);
    listen_for_input(&shell);
    cleanup_shell(&shell);

    return 0;
}
