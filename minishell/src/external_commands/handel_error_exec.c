#include "../minishell.h"

void handle_exec_error(char *cmd, char *message, int exit_code)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(cmd, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    ft_putendl_fd(message, STDERR_FILENO);
    exit(exit_code);
}

int get_exec_error_code(int err)
{
    if (err == ENOENT)
        return 127; // Command not found
    if (err == EACCES)
        return 126; // Permission denied
    return 1;       // General execution error
}

void quit_program(int exit_code)
{
    printf("Exiting program with code: %d\n", exit_code);
    exit(exit_code);
}