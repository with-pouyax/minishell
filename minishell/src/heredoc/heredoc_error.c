#include "../minishell.h"

int	syntax_error_newline(void)
{
	ft_putstr_fd("syntax error near unexpected token "
		"`newline'\n", STDERR_FILENO);
	return (1);
}


int	heredoc_open_error(char *tmp_filename)
{
	write(STDERR_FILENO, "minishell: failed to open temporary file\n", 41);
	free(tmp_filename);
	return (1);
}