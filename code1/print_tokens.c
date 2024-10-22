#include "minishell.h"

// Function to print tokens
void print_tokens(t_token *token_list)
{
    t_token *token;

    token = token_list;
    while (token)
    {
        printf("Token: '%s', Index: %d\n", token->value, token->index);
        if (token->is_command)
        {
            printf(" - This is a command.\n");
            if (token->is_int)
                printf(" - This is an internal command.\n");
            else
                printf(" - This is an external command.\n");
        }
        if (token->is_flag)
            printf(" - This is a flag.\n");
        if (token->is_operator)
        {
            printf(" - This is an operator.\n");
            if (token->wrong_operator)
                printf(" - Warning: This is an invalid operator.\n");
        }
        if (token->var_not_found)
            printf(" - Variable not found during expansion.\n");
        token = token->next;
    }
}


// Function to print commands and their tokens
void	print_commands(t_shell *shell)
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
