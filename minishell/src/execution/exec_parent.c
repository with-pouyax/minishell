#include "../minishell.h"

void	execute_parent(t_shell_data *shell)
{
	t_pid_node	*current;
	pid_t		pid;
	int		status;

	current = shell->pid_list;
	while (current != NULL)
	{
		pid = current->pid;
		if (waitpid(pid, &status, 0) == -1)
			perror("waitpid failed");
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
