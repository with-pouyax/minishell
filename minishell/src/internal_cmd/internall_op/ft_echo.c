// ft_echo.c
#include "internal_commands.h"

int	ft_echo(t_shell_data *shell,t_command *cmd)
{
	t_token	*token;
	int		newline;

	token = cmd->token_list->next; // Skip the command token
	newline = 1;

	// Handle multiple -n options
	while (token && ft_strcmp(token->value, "-n") == 0)
	{
		newline = 0;
		token = token->next;
	}

	// Print arguments
	while (token)
	{
		printf("%s", token->value);
		if (token->next)
			printf(" ");
		token = token->next;
	}

	if (newline)
		printf("\n");

	shell->exit_status = 0;
	return (0);
}
