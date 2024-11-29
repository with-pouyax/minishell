#include "../../minishell.h"

static t_command	*get_last_command(t_shell_data *shell)
{
	t_command	*cmd;

	cmd = shell->commands;
	while (cmd && cmd->next)
		cmd = cmd->next;
	return (cmd);
}

static int	get_end_index(t_token *last_token)
{
	if (last_token)
	{
		while (last_token->next)
			last_token = last_token->next;
		return (last_token->index + 1);
	}
	return (0);
}

static void	handle_mem_error(t_shell_data *shell, char *end_token_str)
{
	ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
	free(end_token_str);
	free_commands(shell);
	shell->commands = NULL;
}


void	append_end_token(t_shell_data *shell)
{
	t_command	*cmd;
	t_token		*last_token;
	char		*end_token_str;
	int			end_index;

	if (!shell->commands)
		return ;
	cmd = get_last_command(shell);
	last_token = cmd->token_list;
	end_index = get_end_index(last_token);
	end_token_str = ft_strdup("\0");
	if (!end_token_str)
	{
		handle_mem_error(shell, end_token_str);
		return ;
	}
	if (add_token(end_token_str, &cmd->token_list, &end_index, 0))
	{
		handle_mem_error(shell, end_token_str);
		return ;
	}
	last_token = cmd->token_list;
	while (last_token->next)
		last_token = last_token->next;
	last_token->is_end = 1;
}
