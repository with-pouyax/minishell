#include "../minishell.h"

void handle_ctrl_d(void)
{
    write(1, "exit\n", 5);
    printf("Debug: shell->input is NULL, exiting shell\n");
}

// return 1 : Skip further processing
// return 0 : Continue processing

int check_input_length(t_shell_data *shell)
{
    if (ft_strlen(shell->input) > MAX_INPUT_LENGTH)
    {
        ft_putstr_fd("minishell: input too long\n", STDERR_FILENO);
        free(shell->input);
        return (1);
    }
    return (0);
}
// return 1 : Skip further processing
// return 0 : Continue processing

int handle_allocation(t_shell_data *shell)
{
    shell->full_input = ft_strdup(shell->input);
    if (!shell->full_input)
    {
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
        free(shell->input);
        return (1);
    }
    return (0);
}

int check_syntax_error(t_shell_data *shell, const char *error_message)
{
    if (error_message)
    {
        ft_putstr_fd(error_message, STDERR_FILENO);
        free(shell->input);
        free(shell->full_input);
        shell->input = NULL;
        shell->full_input = NULL;
        shell->exit_status = 2;
        return (1); // Skip further processing
    }
    return (0); // Continue processing
}

// return (1) : Indicating input was not successful (CTRL+D or error)
// return (0) : Indicating input was successfully read

int read_input(t_shell_data *shell)
{
    shell->input = readline(PROMPT);
    if (!shell->input)
    {
        handle_ctrl_d();
        return (1); 
    }
    return (0);
}


void handle_input(t_shell_data *shell)
{
    int skip_processing;

    skip_processing = 0;
    while (1)
    {
        if (read_input(shell))
            break;
        skip_processing = check_input_length(shell);
        if (!skip_processing)
            skip_processing = handle_allocation(shell);
        if (ft_strlen(shell->full_input) > 0)
            add_history(shell->full_input);
        if (!skip_processing && check_unclosed_quotes(shell->input))
            skip_processing = check_syntax_error(shell, "minishell: syntax error: unclosed quotes\n");
        if (!skip_processing && check_trailing_pipe(shell->input))
            skip_processing = check_syntax_error(shell, "minishell: syntax error near unexpected token `|'\n");
        process_input(shell);
        printf("Debug: Starting execution()---------------------------------------\n");
        execution(shell);
        printf("Debug: Finished execution()\n");
        free_shell_resources(shell);
    }
    rl_clear_history();
}
