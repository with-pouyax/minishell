#include "../../minishell.h"


void expand_variables_in_input(t_shell_data *shell)
{
    char *expanded_input;
    int var_not_found_flag;

    var_not_found_flag = 0;
    expanded_input = expand_variables_in_token(shell ,shell->input, &var_not_found_flag); // Expand the variables in the input and store the result in expanded_input
    if (!expanded_input)
    {
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
        free(shell->input);
        shell->input = NULL;
        shell->exit_status = 1;   //check----------------------------------------
        return;
    }
    free(shell->input);
    shell->input = expanded_input;
}