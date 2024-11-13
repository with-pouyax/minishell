#include "minishell.h"

void preprocess_input(void)
{
    g_data.commands = NULL;
    tokenize_input(); // Tokenize the input string after variable expansion

    if (!g_data.commands)
        return;

    process_heredocs(); // Process heredocs after tokenization

    if (g_data.error_flag)
    {
        //free_commands(); //##important
        g_data.commands = NULL;
        return;
    }

    parse_tokens();
    print_commands(); // ##debug print
    //free_commands();  //##important
}




