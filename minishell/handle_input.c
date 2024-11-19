#include "minishell.h"

void	process_input(void)
{
	expand_variables_in_input(); // Expand variables in input
	preprocess_input(); // Preprocess input (tokenize, parse)
}

void handle_input(void)
{
    printf("Debug: Starting handle_input()\n");

    while (1)
    {
        // printf("Debug: Entering input loop\n");
        // fflush(stdout);

        g_data.input = readline(PROMPT);
        // printf("Debug: Readline complete. g_data.input = '%s'\n", g_data.input);

        if (!g_data.input) // If input is NULL (Ctrl-D), exit the shell
        {
            write(1, "exit\n", 5);
            printf("Debug: g_data.input is NULL, exiting shell\n");
            break;
        }

        if (ft_strlen(g_data.input) > MAX_INPUT_LENGTH)
        {
            ft_putstr_fd("minishell: input too long\n", STDERR_FILENO);
            // printf("Debug: Input length exceeded MAX_INPUT_LENGTH\n");
            free(g_data.input);
            // printf("Debug: Freed g_data.input after exceeding MAX_INPUT_LENGTH\n");
            continue;
        }

        g_data.full_input = ft_strdup(g_data.input);
        printf("Debug: full_command= '%s'\n", g_data.full_input);

        if (!g_data.full_input)
        {
            ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
            // printf("Debug: Memory allocation error for g_data.full_input\n");
            free(g_data.input);
            // printf("Debug: Freed g_data.input after memory allocation error\n");
            continue;
        }

        if (ft_strlen(g_data.full_input) > 0)
        {
            add_history(g_data.full_input);
            // printf("Debug: Added input to history: '%s'\n", g_data.full_input);
        }

        if (check_unclosed_quotes(g_data.input))
        {
            ft_putstr_fd("minishell: syntax error: unclosed quotes\n", STDERR_FILENO);
            // printf("Debug: Unclosed quotes detected in input\n");
            free(g_data.input);
            // printf("Debug: Freed g_data.input after detecting unclosed quotes\n");
            free(g_data.full_input);
            // printf("Debug: Freed g_data.full_input after detecting unclosed quotes\n");
            g_data.input = NULL;
            g_data.full_input = NULL;
            g_data.exit_status = 2;
            continue;
        }

        if (check_trailing_pipe(g_data.input))
        {
            ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
            // printf("Debug: Trailing pipe detected in input\n");
            free(g_data.input);
            // printf("Debug: Freed g_data.input after detecting trailing pipe\n");
            free(g_data.full_input);
            // printf("Debug: Freed g_data.full_input after detecting trailing pipe\n");
            g_data.input = NULL;
            g_data.full_input = NULL;
            g_data.exit_status = 2;
            continue;
        }

        // printf("Debug: Passing input to process_input()\n");
        // printf("Debug: Starting process_input()\n");
        // fflush(stdout);
        process_input();
        // printf("Debug: Finished process_input()\n");
        // fflush(stdout);

        printf("Debug: Starting execution()---------------------------------------\n");
        execution(); // Add execution logic here
        printf("Debug: Finished execution()\n");

		free_commands(); // ##important


        free(g_data.input);
        printf("Debug: Freed g_data.input after process_input()\n");
        g_data.input = NULL;
        free(g_data.full_input);
        printf("Debug: Freed g_data.full_input after process_input()\n");
        g_data.full_input = NULL;

        printf("Debug: Completed processing input\n");
    }

    rl_clear_history();
    printf("Debug: Exiting handle_input(), clearing history\n");
}
