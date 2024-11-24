#include "../minishell.h"

void preprocess_input(t_shell_data *shell)
{
    shell->commands = NULL;
    split_cmd_tokenize(shell);
    if (!shell->commands)
        return;
    if (shell->error_flag) // If an error occurred during heredoc processing
    {
        free_commands(shell); //##important
        shell->commands = NULL;
        return;
    }
    parse_tokens(shell);
    print_commands(shell); // ##debug print
    //free_commands(shell);  //##important
}




