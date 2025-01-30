/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_pids.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:59:12 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/24 14:20:11 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	store_pids(t_shell_data *shell, pid_t pid)
{
	t_pid_node	*new_node;
	t_pid_node	*current;

	new_node = (t_pid_node *)malloc(sizeof(t_pid_node));
	if (!new_node)
	{
		perror("Failed to allocate memory for PID node");
		ft_clean(shell);
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
