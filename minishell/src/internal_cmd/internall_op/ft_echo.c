#include "../internal_commands.h"

/*
while (token && ft_strcmp(token->value, "-n") == 0)-> Handle multiple -n options
while (token)                                      -> Print arguments
Only print the newline if we're in interactive mode
    	if (newline && shell->interactive_mode)

*/
int	ft_echo(t_shell_data *shell, t_command *cmd)
{
	t_token	*token;
	int		newline;

	token = cmd->token_list->next;
	newline = 1;
	while (token && ft_strcmp(token->value, "-n") == 0)
	{
		newline = 0;
		token = token->next;
	}
	while (token)
	{
		printf("%s", token->value);
		if (token->next)
			printf(" ");
		token = token->next;
	}
    if (newline && shell->interactive_mode)
    {
		printf("\n");
	}
	return (0);
}
