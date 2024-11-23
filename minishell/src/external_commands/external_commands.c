
#include "minishell.h"


void    execute_external_commands(t_shell_data *shell)
{
    char    *cmd_path;
	pid_t 	pid;
	char    **argv;

	argv = convert_tokens_to_argv(shell->commands->token_list);
    if (!argv || !argv[0])                        // Empty command check
        return;
    cmd_path = get_command_path(shell, shell->commands->token_list);
    if (!cmd_path)
		handle_exec_error(argv[0], "command not found", 127);
	pid = fork();
    if (pid < 0)
        quit_program(EXIT_FAILURE);
    if (pid == 0)
        exec_external_child(shell, cmd_path, argv);
    free(cmd_path);
    free(argv);
}
