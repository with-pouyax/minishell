#include "minishell.h"

t_shell g_shell;

void    shell_loop(void)
{
    char    *input;

    load_history();
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
                execute_commands();
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
    (void)argc;
    (void)argv;
    init_shell(envp);
    shell_loop();
    return (g_shell.last_exit_status);
}
