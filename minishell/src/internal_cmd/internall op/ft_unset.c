// ft_unset.c
#include "internal_commands.h"

int	ft_unset(t_command *cmd)
{
	t_token	*token;

	token = cmd->token_list->next; // Skip the command token

	while (token)
	{
		if (is_valid_identifier(token->value))
			remove_from_env(token->value);
		else
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(token->value, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			g_data.exit_status = 1;
		}
		token = token->next;
	}

	return (0);
}
