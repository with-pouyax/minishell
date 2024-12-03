#include "../minishell.h"

void	handle_exec_error(t_shell_data *shell, char *cmd, char *message,
			int exit_code)
{
	// ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(message, STDERR_FILENO);
	shell->exit_status = exit_code;
}

void	write_error(char *exec_name, char *err_message)
{
	// ft_putstr_fd("msh: ", STDERR_FILENO);
	ft_putstr_fd(exec_name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(err_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	get_exec_error_code(int err)
{
	if (err == ENOENT)
		return (127);
	if (err == EACCES)
		return (126);
	return (1);
}

void	quit_program(int exit_code)
{
	printf("Exiting program with code: %d\n", exit_code);
	exit(exit_code);
}
