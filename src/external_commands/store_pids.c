#include "../../include/minishell.h"

void	store_pids(t_shell_data *shell, pid_t pid)
{
	t_pid_node	*new_node;
	t_pid_node	*current;

	new_node = (t_pid_node *)malloc(sizeof(t_pid_node));
	if (!new_node)
	{
		perror("Failed to allocate memory for PID node");
		return ;
	}
	new_node->pid = pid;
	new_node->next = NULL;
	if (shell->pid_list == NULL)
	{
		shell->pid_list = new_node;
	}
	else
	{
		current = shell->pid_list;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}
