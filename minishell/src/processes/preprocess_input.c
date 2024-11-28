#include "../minishell.h"

void preprocess_input(t_shell_data *shell)
{
    shell->commands = NULL;
    split_cmd_tokenize(shell);
    if (!shell->commands)
        return;
    if (shell->error_flag)
    {
        free_commands(shell); // Important: Free commands if an error flag is set
        shell->commands = NULL;
        return;
    }
    parse_tokens(shell);

    // Validate operators after parsing tokens
    if (validate_operators(shell))
    {
        // If a syntax error was found, skip further processing
        return;
    }

    print_commands(shell); // Debug print
    // free_commands(shell);  // Optional: Free commands after validation if needed
}





