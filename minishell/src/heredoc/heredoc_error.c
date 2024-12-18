#include "../minishell.h"

int	syntax_error_newline(void)
{
	ft_putstr_fd("syntax error near unexpected token "
		"`newline'\n", STDERR_FILENO);
	return (1);
}


int	heredoc_open_error(char *tmp_filename)
{
	perror("minishell: heredoc");
	free(tmp_filename);
	return (1);
}