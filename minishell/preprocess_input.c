#include "minishell.h"

void preprocess_input(t_shell_data *shell)
{
    shell->commands = NULL;
    split_cmd_tokenize(shell);
    if (!shell->commands)
        return;
    process_heredocs(); // Process heredocs after tokenization
    if (shell->error_flag) // If an error occurred during heredoc processing
    {
        free_commands(); //##important
        shell->commands = NULL;
        return;
    }
    parse_tokens();
    // print_commands(); // ##debug print
    //free_commands();  //##important
}




