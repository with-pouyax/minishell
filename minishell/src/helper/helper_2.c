#include "../minishell.h"

void	skip_spaces(t_shell_data *shell, int *i)
{
	while (shell->input[*i] && ft_isspace(shell->input[*i]))
		(*i)++;
}
