#include "minishell.h"

void preprocess_input(t_shell_data *shell)
{
    shell->commands = NULL;
    tokenize_input(shell); //ok
    if (!shell->commands)
        return;
    process_heredocs(shell); //ok
    if (shell->error_flag) // If an error occurred during heredoc processing
    {
        free_commands(shell); //##important
        shell->commands = NULL;
        return;
    }
    parse_tokens(shell); //ok
    print_commands(shell); //ok 
    //free_commands();  //##important
}




