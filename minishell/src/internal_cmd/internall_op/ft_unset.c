// ft_unset.c
#include "internal_commands.h"

int	ft_unset(t_shell_data *shell,t_command *cmd)
{
	t_token	*token;

	token = cmd->token_list->next; // Skip the command token

	while (token)
	{
		if (is_valid_identifier(token->value))
			remove_from_env(shell, token->value);
		else
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(token->value, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			shell->exit_status = 1;
		}
		token = token->next;
	}

	return (0);
}