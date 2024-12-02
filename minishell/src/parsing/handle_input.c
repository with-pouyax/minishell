#include "../minishell.h"

void handle_ctrl_d(t_shell_data *shell)
{
    write(1, "exit\n", 5);
    cleanup(shell); // Free all allocated resources
    exit(0);        // Exit the shell
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


// return (1) : Indicating input was not successful (CTRL+D or error)
// return (0) : Indicating input was successfully read

int read_input(t_shell_data *shell)
{
    shell->input = readline(PROMPT);
    if (!shell->input)
    {
        handle_ctrl_d(shell);
        return (1); // Break the loop
    }
    return (0);
}

void handle_input(t_shell_data *shell)
{
    while (1)
    {
        if (read_input(shell))
            break;

        if (shell->input == NULL || shell->input[0] == '\0')
        {
            free(shell->input);
            shell->input = NULL;
            // Reset signal status if necessary
            g_signal_status = 0;
            // Continue to prompt again by skipping the rest of the loop
        }
        else
        {
            // Reset signal status at the start of processing
            g_signal_status = 0;

            // Check input length
            if (check_input_length(shell))
            {
                free_shell_resources(shell);
                // Skip processing and prompt again
            }
            else
            {
                // Handle allocation
                if (handle_allocation(shell))
                {
                    free_shell_resources(shell);
                    // Skip processing and prompt again
                }
                else
                {
                    // Add to history if input is not empty
                    if (ft_strlen(shell->full_input) > 0)
                        add_history(shell->full_input);

                    // Syntax checks
                    if (check_unclosed_quotes(shell->input))
                    {
                        check_syntax_error(shell, "minishell: syntax error: unclosed quotes\n");
                        free_shell_resources(shell);
                        // Skip processing and prompt again
                    }
                    else if (check_leading_pipe(shell->input))
                    {
                        check_syntax_error(shell, "minishell: syntax error near unexpected token `|'\n");
                        free_shell_resources(shell);
                        // Skip processing and prompt again
                    }
                    else if (check_trailing_pipe(shell->input))
                    {
                        check_syntax_error(shell, "minishell: syntax error near unexpected token `|'\n");
                        free_shell_resources(shell);
                        // Skip processing and prompt again
                    }
                    else
                    {
                        // Process the input
                        process_input(shell);

                        printf("\nDebug: shell->cmds_nb: %d\n\n", shell->cmds_nb);
                        printf("\nDebug: shell->pipe_nb: %d\n\n", shell->pipe_nb);

                        // Conditional Execution Starts Here
                        if (shell->commands)
                        {
                            printf("Debug: Starting execution()---------------------------------------\n");
                            execution(shell);
                            // printf("Debug: Finished execution()-------------------------------------\n");
                        }
                        else
                        {
                            // If commands are NULL, it means there was a syntax error or no commands to execute
                            shell->exit_status = 2; // Optional: Ensure exit status reflects the error
                        }
                        // Conditional Execution Ends Here

                        // Free resources after processing
                        free_shell_resources(shell);
                    }
                }
            }
        }
    }
    rl_clear_history();
}

