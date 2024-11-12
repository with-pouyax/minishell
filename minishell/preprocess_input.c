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

    if (g_data.full_input && ft_strlen(g_data.full_input) > 0)
        add_history(g_data.full_input);

    parse_tokens();
    print_commands(); // ##debug print
    //free_commands();  //##important
}



