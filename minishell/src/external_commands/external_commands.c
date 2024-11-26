
#include "../minishell.h"

/*
Flow of Execution
Parent Process:

Forks a child.
Tracks the child's PID for cleanup later.
Child Process:

Closes unused pipes.
Executes the command using execve().
On failure, exits with the appropriate error code.
Post-Execution:

After all commands in a pipeline are forked, the parent waits for all child processes using waitpid().
*/

int token_list_length(t_token *token)
{
    int count;
	
	count = 0;
    while (token)
    {
        if (!token->is_end)
            count++;
        token = token->next;
    }
    return (count);
}

char **convert_tokens_to_argv(t_token *token_list)
{
    int 	count;
	int		i;
    char	**argv;

	i = 0;

    printf("Token list:\n");
    t_token *temp = token_list;
    while (temp) {
        printf("Token value: '%s', Index: %d\n", temp->value, temp->index);
        temp = temp->next;
    }
	count = token_list_length(token_list);
    printf("Number of tokens: %d\n", count);
	argv = malloc(sizeof(char *) * (count + 1));
    if (!argv)
        exit(EXIT_FAILURE);
    while (i < count && token_list)
    {
        argv[i] = ft_strdup(token_list->value);
        if (!argv[i])
            exit(EXIT_FAILURE);
        token_list = token_list->next;
		i++;
    }
    argv[count] = NULL;
    return (argv);
}

void exec_external_child(t_shell_data *shell, char *cmd_path, char **argv)
{
    int error_code;

    printf("start child proces");
    close_all_pipes(shell->pipes, shell->cmds_nb);
    // wait_for_all_children(shell);
    if (execve(cmd_path, argv, shell->envp) == -1)
    {
        error_code = get_exec_error_code(errno);
        handle_exec_error(argv[0], strerror(errno), error_code);
        printf("the cmd not executes");
    }
}

void    execute_external_commands(t_shell_data *shell)
{
    char    *cmd_path;
    char    **arr_token;
	pid_t 	pid;

	arr_token = convert_tokens_to_argv(shell->commands->token_list);
    if (!arr_token || !arr_token[0])
        return;
    cmd_path = get_command_path(shell, shell->commands->token_list);
    if (!cmd_path)
	{
        handle_exec_error(arr_token[0], "command not found", 127);
        return;
    }
	pid = fork();
    if (pid < 0)
        quit_program(EXIT_FAILURE);
    else if (pid == 0)
        exec_external_child(shell, cmd_path, arr_token);
    else
    {
        int status;
        shell->commands->pid = pid;
        waitpid(pid, &status, 0);  // Wait for the child process to terminate
        // Optionally check the child's exit status
        if (WIFEXITED(status)) {
            // printf("Child process %d exited with status %d\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            // printf("Child process %d was terminated by signal %d\n", pid, WTERMSIG(status));  
        }
    }
    store_pids(shell, pid);
    free(cmd_path);
    free(arr_token);
}
