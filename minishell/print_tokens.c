#include "src/minishell.h"

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
		// print_token_details(token);
		token = token->next;
	}
}


void	print_commands(t_shell_data *shell)
{
	t_command	*cmd;

	cmd = shell->commands;
	while (cmd)
	{
		printf("Command index:%d command_nb :%d pipe_nb :%d '%s' full_cmd; %s\n", cmd->index, cmd->cmds_nb, cmd->pipe_nb, cmd->command_string, shell->full_input);
		print_tokens(cmd->token_list);
		cmd = cmd->next;
	}
}


// int debug_print_commands(t_shell_data *shell)
// {
//     t_command *cmd = shell->commands;
//     t_token *token;
//     int command_count = 0;

//     if (!cmd)
//     {
//         printf("No commands to execute.\n");
//         return (0);
//     }

//     while (cmd)
//     {
//         command_count++;
//         printf("Executing Command #%d: %s\n", cmd->index, cmd->command_string);
//         token = cmd->token_list;
//         while (token)
//         {
//             printf("execute_commands: Function called.\n");
//             printf("  Token #%d: '%s'\n", token->index, token->value);
            
//             // Detailed Token Properties
//             if (token->is_command)
//                 printf("    - Type: Command\n");
//             if (token->is_builtin)
//                 printf("    - Builtin: Yes\n");
//             if (token->is_flag)
//                 printf("    - Type: Flag\n");
//             if (token->is_argument)
//                 printf("    - Type: Argument\n");
//             if (token->is_operator)
//                 printf("    - Type: Operator\n");
//             if (token->is_file)
//                 printf("    - Type: File\n");
//             if (token->is_variable_expansion)
//                 printf("    - Type: Variable Expansion\n");
//             if (token->is_env_assignment)
//                 printf("    - Type: Environment Assignment\n");
//             if (token->is_int)
//                 printf("    - Internal Command: Yes\n");
//             if (token->var_not_found)
//                 printf("    - Variable Not Found: Yes\n");
//             if (token->wrong_operator)
//                 printf("    - Warning: Invalid Operator Detected\n");
//             if (token->is_heredoc)
//             {
//                 printf("    - Type: Heredoc\n");
//                 printf("      * Delimiter: '%s'\n", token->heredoc_delimiter);
//                 printf("      * File: '%s'\n", token->heredoc_file);
//             }

//             token = token->next;
//         }
//         printf("\n"); // Separator between commands
//         cmd = cmd->next;
//     }

//     printf("Total Commands Executed: %d\n", command_count);
//     return (0);
// }
