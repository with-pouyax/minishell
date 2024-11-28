
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

    printf("\n\nToken list:\n");
    t_token *temp = token_list;
    while (temp)
    {
        printf("Token value: '%s', Index: %d\n", temp->value, temp->index);
        temp = temp->next;
    }
	count = token_list_length(token_list);
    printf("Number of tokens: %d\n\n", count);
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
/*
We need to close all the pipes in the child process because: we already 
changed the stdout and stdin inside set_redirs and set_pipes()
*/
// If execve call is successful, the following lines are never executed
// chon : the child process will kill itself when finished

void exec_external_child(t_shell_data *shell, char *cmd_path, char **argv)
{
    int error_code;
    int exit_status;

    printf("start child proces");
    close_all_pipes(shell->pipes, shell->cmds_nb);
    // wait_for_all_children(shell);
    exit_status = execve(cmd_path, argv, shell->envp);
    if (exit_status == -1)
    {
        error_code = get_exec_error_code(errno);
        handle_exec_error(shell, argv[0], strerror(errno), error_code);
        printf("the cmd not executes");
    }
}

void    execute_external_commands(t_shell_data *shell, t_command *cmds)
{
    char    *cmd_path;
    char    **arr_token;
	pid_t 	pid;

    printf("this is external executing\n");
	arr_token = convert_tokens_to_argv(cmds->token_list);
    if (!arr_token || !arr_token[0])
        return;
    cmd_path = get_command_path(shell, cmds->token_list);
    if (!cmd_path)
	{
        handle_exec_error(shell, arr_token[0], "command not found", 127);
        free(arr_token);
        return;
    }
	pid = fork();
    if (pid < 0)
        quit_program(EXIT_FAILURE);
    else if (pid == 0)
    {
        exec_external_child(shell, cmd_path, arr_token);
        exit(shell->exit_status);
    }
    else 
    {
        printf("Parent Process: PID = %d, Child PID = %d\n\n\n", getpid(), pid);
        store_pids(shell, pid);
    }
    free(cmd_path);
    free(arr_token);
}
