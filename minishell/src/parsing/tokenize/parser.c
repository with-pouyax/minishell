#include "../../minishell.h"

void	parse_tokens(t_shell_data *shell)
{
	t_command	*cmd;
	t_token		*token;

	cmd = shell->commands;
	while (cmd)
	{
		token = cmd->token_list;
		while (token)
		{
			if (token->index == 0 && !token->is_operator)
			{
				token->is_command = 1;
				if (is_internal_command(token->value))
					token->is_int = 1;
			}
			else if (ft_strncmp(token->value, "-", 1) == 0)
				token->is_flag = 1;
			token = token->next;
		}
		cmd = cmd->next;
	}
}
