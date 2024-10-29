#include "minishell.h"

void preprocess_input(void)
{
    char *expanded_input;
    int var_not_found_flag = 0;

    expanded_input = expand_variables_in_token(g_data.input, &var_not_found_flag);
    if (!expanded_input)
    {
        ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
    free(g_data.input);
    g_data.input = expanded_input;
}
