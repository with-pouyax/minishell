#include "minishell.h"

t_shell g_shell; // Global shell structure to store environment, last exit status, and commands

void    shell_loop(void)
{
    char    *input;

    load_history(); // Load the history file
    while (1)
    {
        signal_setup();
        input = readline("minishell$ ");
        if (input == NULL)
        {
            write(STDERR_FILENO, "exit\n", 5);
            break ;
        }
        if (*input != '\0')
        {
            add_to_history(input);
            input = expand_env_vars(input);
            if (parse_input(input) == 0)
            {
                execute_command(g_shell.cmd_list);

                free_command_list();
            }
            else
                g_shell.last_exit_status = 2;
        }
        free(input);
    }
    save_history();
    cleanup_shell();
}

int main(int argc, char **argv, char **envp)
{
    (void)argc; // Unused
    (void)argv; // Unused
    init_shell(envp); // Initialize the shell with the environment variables
    shell_loop(); // Start the main shell loop
    return (g_shell.last_exit_status);
}
