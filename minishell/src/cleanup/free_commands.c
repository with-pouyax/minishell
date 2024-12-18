#include "../minishell.h"

void	free_tokens(t_token *token_list)
{
	t_token	*token;
	t_token	*next_token;

	token = token_list;
	while (token)
	{
		next_token = token->next;
		free(token->value);
		if (token->original_value)
			free(token->original_value);
		if (token->is_heredoc)
			free_heredoc_token(token);
		free(token);
		token = next_token;
	}
}

void	free_heredoc_token(t_token *token)
{
	if (token->heredoc_file)
	{
		unlink(token->heredoc_file);
		free(token->heredoc_file);
	}
	if (token->heredoc_delimiter)
		free(token->heredoc_delimiter);
}

void	free_shell_resources(t_shell_data *shell)
{
	free_commands(shell);
	free(shell->input);
	free(shell->full_input);
	shell->input = NULL;
	shell->full_input = NULL;
}

void	free_commands(t_shell_data *shell)
{
	t_command	*cmd;
	t_command	*next_cmd;

	cmd = shell->commands;
	while (cmd)
	{
		next_cmd = cmd->next;
		free_tokens(cmd->token_list);
		free_redirections(cmd->redirections);
		free(cmd->command_string);
		free(cmd);
		cmd = next_cmd;
	}
	shell->commands = NULL;
}
