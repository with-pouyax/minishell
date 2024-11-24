#include "src/minishell.h"

void print_horizontal_line(int length) {
    for (int i = 0; i < length; i++)
        printf("-");
    printf("\n");
}

// Function to print table headers with alignment
void print_table_header(const char *headers[], int num_columns, int widths[]) {
    printf("|");
    for (int i = 0; i < num_columns; i++) {
        printf(" %-*s |", widths[i] - 2, headers[i]);
    }
    printf("\n");
}

// Function to print a single row of the table
void print_table_row(const char *row_data[], int num_columns, int widths[]) {
    printf("|");
    for (int i = 0; i < num_columns; i++) {
        printf(" %-*s |", widths[i] - 2, row_data[i]);
    }
    printf("\n");
}


void print_redirections(t_redirection *redirs)
{
    const char *redir_headers[] = {"Type", "Redir#", "Filename", "Delimiter", "Heredoc File"};
    int redir_widths[] = {15, 8, 15, 15, 20};
    int num_redir_columns = sizeof(redir_headers) / sizeof(redir_headers[0]);

    print_horizontal_line(80);
    print_table_header(redir_headers, num_redir_columns, redir_widths);
    print_horizontal_line(80);

    while (redirs)
    {
        const char *type_str;
        switch (redirs->type)
        {
            case REDIR_INPUT:
                type_str = "REDIR_INPUT";
                break;
            case REDIR_OUTPUT:
                type_str = "REDIR_OUTPUT";
                break;
            case REDIR_APPEND:
                type_str = "REDIR_APPEND";
                break;
            case REDIR_HEREDOC:
                type_str = "REDIR_HEREDOC";
                break;
            default:
                type_str = "UNKNOWN";
        }

        char redir_number_str[10];
        snprintf(redir_number_str, sizeof(redir_number_str), "%d", redirs->redir_number);

        const char *redir_row[] = {
            type_str,
            redir_number_str,
            redirs->filename ? redirs->filename : "NULL",
            redirs->delimiter ? redirs->delimiter : "NULL",
            redirs->heredoc_file ? redirs->heredoc_file : "NULL"
        };

        print_table_row(redir_row, num_redir_columns, redir_widths);
        redirs = redirs->next;
    }
    print_horizontal_line(80);
}



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

void print_tokens(t_token *token_list)
{
    const char *token_headers[] = {"Index", "Value", "Type", "Original Value"};
    int token_widths[] = {7, 15, 20, 25};
    int num_token_columns = sizeof(token_headers) / sizeof(token_headers[0]);

    print_horizontal_line(70); // Adjusted for wider table
    print_table_header(token_headers, num_token_columns, token_widths);
    print_horizontal_line(70);

    while (token_list)
    {
        char token_index_str[10];
        char original_value_str[25];

        // Prepare the index string
        snprintf(token_index_str, sizeof(token_index_str), "%d", token_list->index);

        // Prepare the original value string
        if (token_list->original_value)
            snprintf(original_value_str, sizeof(original_value_str), "%s", token_list->original_value);
        else
            snprintf(original_value_str, sizeof(original_value_str), "NULL");

        // Determine the type of token
        char type_str[20] = "";
        if (token_list->is_command)
            strcat(type_str, "Command ");
        if (token_list->is_flag)
            strcat(type_str, "Flag ");
        if (token_list->is_operator)
            strcat(type_str, "Operator ");
        if (token_list->is_end)
            strcat(type_str, "End ");
        if (token_list->is_heredoc)
            strcat(type_str, "Heredoc ");
        if (token_list->var_not_found)
            strcat(type_str, "VarNotFound ");

        const char *token_row[] = {
            token_index_str,
            token_list->value,
            type_str,
            original_value_str
        };

        print_table_row(token_row, num_token_columns, token_widths);
        token_list = token_list->next;
    }
    print_horizontal_line(70);
}






void print_commands(t_shell_data *shell)
{
    t_command *cmd;

<<<<<<< HEAD
	cmd = shell->commands;
	while (cmd)
	{
		printf("Command index:%d command_nb :%d pipe_nb :%d '%s' full_cmd; %s\n", cmd->index, shell->cmds_nb, shell->pipe_nb, cmd->command_string, shell->full_input);
		print_tokens(cmd->token_list);
		cmd = cmd->next;
	}
=======
    cmd = shell->commands;
    while (cmd)
    {
        printf("\n=========================================\n");
        printf("Command #%d\n", cmd->index);
        printf("=========================================\n");

        // Define headers and their respective widths
        const char *cmd_headers[] = {"Index", "Cmds#", "Pipes#", "Command String", "Full Command"};
        int cmd_widths[] = {7, 7, 8, 25, 50};
        int num_cmd_columns = sizeof(cmd_headers) / sizeof(cmd_headers[0]);

        // Print command table header
        print_horizontal_line(100);
        print_table_header(cmd_headers, num_cmd_columns, cmd_widths);
        print_horizontal_line(100);

        // Prepare row data
        char index_str[10];
        char cmds_nb_str[10];
        char pipe_nb_str[10];
        snprintf(index_str, sizeof(index_str), "%d", cmd->index);
        snprintf(cmds_nb_str, sizeof(cmds_nb_str), "%d", cmd->cmds_nb);
        snprintf(pipe_nb_str, sizeof(pipe_nb_str), "%d", cmd->pipe_nb);

        const char *cmd_row[] = {
            index_str,
            cmds_nb_str,
            pipe_nb_str,
            cmd->command_string,
            shell->full_input
        };

        // Print command row
        print_table_row(cmd_row, num_cmd_columns, cmd_widths);
        print_horizontal_line(100);

        // Print Tokens
        printf("\nTokens:\n");
        print_tokens(cmd->token_list);

        // Print Redirections
        printf("\nRedirections:\n");
        print_redirections(cmd->redirections);

        cmd = cmd->next;
    }
>>>>>>> d8a31fee03d36587b120b4562ac5b45af3b4a40d
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
