#include "minishell.h"

void	print_token_details(t_token *token)
{
	if (token->is_operator)
		print_operator_token(token);
	else if (token->is_command)
		print_command_token(token);
	if (token->is_flag)
		printf(" - This is a flag.\n");
    
    if (token->original_value)
        printf("    - Original Value: '%s'\n", token->original_value);
}

void	print_operator_token(t_token *token)
{
	printf(" - This is an operator.\n");
	if (token->wrong_operator)
		printf(" - Warning: Invalid operator detected.\n");
	if (token->is_heredoc)
	{
		printf(" - This is a heredoc operator.\n");
		printf(" - Heredoc delimiter: '%s'\n", token->heredoc_delimiter);
		printf(" - Heredoc file: '%s'\n", token->heredoc_file);
	}
}

void	print_command_token(t_token *token)
{
	printf(" - This is a command.\n");
	if (token->is_int)
		printf(" - This is an internal command.\n");
	else
		printf(" - This is an external command.\n");
}

void	print_tokens(t_token *token_list)
{
	t_token	*token;

	token = token_list;
	while (token)
	{
		printf("Token: '%s', Index: %d\n", token->value, token->index);
		print_token_details(token);
		token = token->next;
	}
}


void	print_commands(t_shell_data *shell)
{
	t_command	*cmd;

	cmd = shell->commands;
	while (cmd)
	{
		printf("Command #%d: '%s'\n", cmd->index, cmd->command_string);
		print_tokens(cmd->token_list);
		cmd = cmd->next;
	}
}
