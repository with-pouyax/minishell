#include "minishell.h"

void preprocess_input(void)
{
    char *expanded_input;

    g_data.var_not_found_flag = 0;
    g_data.expansion_input = g_data.input;
    expanded_input = expand_variables_in_token();
    if (!expanded_input)
    {
        ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
        free(g_data.input);
        free(g_data.full_input);
        exit(EXIT_FAILURE);
    }
    free(g_data.input);
    g_data.input = expanded_input;
}



