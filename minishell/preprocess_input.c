#include "minishell.h"

void preprocess_input(void)
{
    g_data.commands = NULL;
    tokenize_input();

    if (!g_data.commands)
        return;

    expand_variables_in_tokens(); // Perform variable expansion on tokens after tokenization.

    process_heredocs(); // Process heredocs after tokenization and variable expansion

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



