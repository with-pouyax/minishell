// ft_export.c
#include "../internal_commands.h"

int	ft_export(t_shell_data *shell,t_command *cmd)
{
	t_token	*token;

	token = cmd->token_list->next; // Skip the command token

	if (!token)
	{
		print_sorted_env(shell);
		shell->exit_status = 0;
		return (0);
	}

	while (token)
	{
		if (is_valid_identifier(token->value))
			add_to_env(shell, token->value);
		else
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(token->value, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			shell->exit_status = 1;
		}
		token = token->next;
	}

	return (0);
}
