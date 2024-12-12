#include "../minishell.h"

void free_argv(char **argv, int count)
{
	int i;

	i = 0;
    while (i < count)
    {
		if (argv[i])
            free(argv[i]);
		i++;
	}
    free(argv);
}

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

After all commands in a pipeline are forked, the parent waits
for all child processes using waitpid().
*/
int	token_list_length(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		if (!token->is_end)
			count++;
		token = token->next;
	}
	return (count);
}

char	*resolve_command_path(t_shell_data *shell, t_command *cmds,
				char **arr_token)
{
	char	*cmd_path;
	struct stat path_stat;

	cmd_path = get_command_path(shell, cmds->token_list);
	if (!cmd_path)
	{
		write_error(arr_token[0], "command not found");
		shell->exit_status = 127;
		free_argv(arr_token, token_list_length(cmds->token_list));
		return (NULL);
	}
	if (cmd_path[0] == '.' && (cmd_path[1] == '\0' || (cmd_path[1] == '.' && cmd_path[2] == '\0')))
    {
        write_error(cmd_path, "command not found");
        shell->exit_status = 127;
        free(cmd_path);
        free_argv(arr_token, token_list_length(cmds->token_list));
        return (NULL);
    }
	if (stat(cmd_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
    {
        write_error(cmd_path, "Is a directory");
        shell->exit_status = 126;
        free(cmd_path);
        free_argv(arr_token, token_list_length(cmds->token_list));
        return (NULL);
    }
	return (cmd_path);
}


int	convert_tokens_to_argv(t_token *token_list, char **argv)
{
    int		i;
    t_token	*temp;

    i = 0;
    temp = token_list;
    while (temp)
    {
        argv[i] = ft_strdup(temp->value);
        if (!argv[i])
        {
            free_argv(argv, i);
            return -1;
        }
        temp = temp->next;
        i++;
    }
    argv[i] = NULL;
    return 0;
}

/*
We need to close all the pipes in the child process because: we already 
changed the stdout and stdin inside set_redirs and set_pipes()

 If execve call is successful, the following lines are never executed
 chon : the child process will kill itself when finished
*/
void	exec_external_child(t_shell_data *shell, char *cmd_path, char **argv)
{
	int	error_code;
	int	exit_status;

	close_all_pipes(shell->pipes, shell->cmds_nb);
	exit_status = execve(cmd_path, argv, shell->envp);
	if (exit_status == -1)
	{
		error_code = get_exec_error_code(errno);
		free(cmd_path);
		write_error(argv[0], strerror(errno));
		free(argv);
		argv = NULL;
		shell->exit_status = error_code;
	}
}

void	execute_external_commands(t_shell_data *shell, t_command *cmds)
{
	char	*cmd_path;
	char	**arr_token;
	pid_t	pid;
	int token_count;

	token_count = token_list_length(cmds->token_list);
    arr_token = malloc(sizeof(char *) * (token_count + 1));
    if (!arr_token)
    {
        write_error("Memory allocation failed", strerror(errno));
        return;
    }
    if (convert_tokens_to_argv(cmds->token_list, arr_token) == -1)
    {
        free(arr_token);
        return;
    }
    if (!arr_token[0])
    {
        free(arr_token);
        return;
    }
	cmd_path = resolve_command_path(shell, cmds, arr_token);
	if (!cmd_path)
		return ;
	pid = fork();
	if (pid < 0)
	{
		write_error("Fork failed", strerror(errno));
		free(cmd_path);
		free(arr_token);
		return ;
	}
	else if (pid == 0)
	{
		exec_external_child(shell, cmd_path, arr_token);
		exit(shell->exit_status);
	}
	else
		store_pids(shell, pid);
	free(cmd_path);
	free(arr_token);
}

void	forking(t_shell_data *shell, t_command *cmds)
{
	setup_signal_handlers(2);
	if (cmds->token_list && cmds->token_list->is_int)
	{
		execute_internal_commands(shell, cmds);
	}
	else
	{
		execute_external_commands(shell, cmds);
	}
}
