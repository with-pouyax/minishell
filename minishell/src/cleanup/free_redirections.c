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
		if (current->heredoc_file)
		{
			unlink(current->heredoc_file);
			free(current->heredoc_file);
		}
		free(current);
		current = next;
	}
}
