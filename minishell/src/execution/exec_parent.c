#include "../minishell.h"

/*
		Save the exit status of the last child process:
(WIFEXITED(status))    --->  Normal termination
(WIFSIGNALED(status))  ---> Terminated by a signal
shell->exit_status = 1 ---> Fallback for unexpected cases
*/
void	execute_parent(t_shell_data *shell)
{
	t_pid_node	*current;
	pid_t		pid;
	int			status;

	current = shell->pid_list;
	while (current != NULL)
	{
		pid = current->pid;
		if (waitpid(pid, &status, 0) == -1)
			perror("waitpid failed");
		else
		{
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->exit_status = 128 + WTERMSIG(status);
			else
				shell->exit_status = 1;
		}
		current = current->next;
	}
	clear_pid_list(shell);
}

void	clear_pid_list(t_shell_data *shell)
{
	t_pid_node	*current;
	t_pid_node	*next_node;

	current = shell->pid_list;
	while (current != NULL)
	{
		next_node = current->next;
		free(current);
		current = next_node;
	}
	shell->pid_list = NULL;
}
