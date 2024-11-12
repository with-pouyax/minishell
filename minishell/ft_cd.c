// ft_cd.c
#include "internal_commands.h"

int	ft_cd(t_command *cmd)
{
	t_token	*token;
	char	*path;

	token = cmd->token_list->next; // Skip the command token

	if (!token)
	{
		path = getenv_from_envp("HOME");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			g_data.exit_status = 1;
			return (1);
		}
	}
	else
		path = token->value;

	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		g_data.exit_status = 1;
		return (1);
	}

	g_data.exit_status = 0;
	return (0);
}
