#include "../minishell.h"


void	free_redirections(t_redirection *redirs)
{
	t_redirection	*current;
	t_redirection	*next;

	current = redirs;
	while (current)
	{
		next = current->next;
		if (current->filename)
			free(current->filename);
		if (current->delimiter)
			free(current->delimiter);
		free(current);
		current = next;
	}
}
