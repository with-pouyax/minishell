#include "minishell.h"

void handle_input(t_shell_data *shell)
{
    printf("Debug: Starting handle_input()\n");

    while (1)
    {
        // printf("Debug: Entering input loop\n");
        // fflush(stdout);

        shell->input = readline(PROMPT);
        // printf("Debug: Readline complete. shell->input = '%s'\n", shell->input);
        if (!shell->input)           // If input is NULL (Ctrl-D), exit the shell
        {
            write(1, "exit\n", 5);
            printf("Debug: shell->input is NULL, exiting shell\n");
            break;
        }
        if (ft_strlen(shell->input) > MAX_INPUT_LENGTH)
        {
            ft_putstr_fd("minishell: input too long\n", STDERR_FILENO);
            free(shell->input);
            continue;       //check what to do with this part--------------------
        }
        shell->full_input = ft_strdup(shell->input);
        printf("Debug: full_command= '%s'\n", shell->full_input);
        if (!shell->full_input)
        {
            ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
            free(shell->input);
            continue;      //check !!!! ----------------------------------------
        }
        if (ft_strlen(shell->full_input) > 0)
        {
            add_history(shell->full_input);
        }
        if (check_unclosed_quotes(shell->input))
        {
            ft_putstr_fd("minishell: syntax error: unclosed quotes\n", STDERR_FILENO);
            free(shell->input);
            free(shell->full_input);
            shell->input = NULL;
            shell->full_input = NULL;
            shell->exit_status = 2;  //check------------------------------------
            continue;                //check------------------------------------
        }
        if (check_trailing_pipe(shell->input))
        {
            ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
            free(shell->input);
            free(shell->full_input);
            shell->input = NULL;
            shell->full_input = NULL;
            shell->exit_status = 2;   //check------------------------------------
            continue;                 //check------------------------------------
        }
        // printf("Debug: Starting process_input()\n");
        process_input(shell);
        // printf("Debug: Finished process_input()\n");
        printf("Debug: Starting execution()---------------------------------------\n");
        execution(); // Add execution logic here
        printf("Debug: Finished execution()\n");

		free_commands(shell); // ##important


        free(shell->input);
        printf("Debug: Freed shell->input after process_input()\n");
        shell->input = NULL;
        free(shell->full_input);
        printf("Debug: Freed shell->full_input after process_input()\n");
        shell->full_input = NULL;

        printf("Debug: Completed processing input\n");
    }

    rl_clear_history();
    printf("Debug: Exiting handle_input(), clearing history\n");
}
