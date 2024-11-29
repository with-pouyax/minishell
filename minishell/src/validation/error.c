#include "../minishell.h"

void	handle_syntax_error(t_shell_data *shell, t_token *token)
{
	ft_putstr_fd("minishell: syntax error unexpected token", STDERR_FILENO);
	ft_putstr_fd(token->value, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	shell->exit_status = 2;
}
