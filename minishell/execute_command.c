#include "minishell.h"




/* Execution Function Stub */

void execute_external_command(t_command *cmd)
{
    printf("This is an external command test MINA: %s\n", cmd->token_list->value);
}

void execute_internal_command(t_command *cmd)
{
    printf("This is an internal command test MINA: %s\n", cmd->token_list->value);
}



/*
Internal Commands: Executed directly within the shell, affecting the shell’s state. Examples: cd, exit.
External Commands: Executed in a new process, independent of the shell’s state. Examples: ls, grep.
*/

/*
issue : the cmd is NULL here
the issue isn't necessarily with recognizing the ls token or command itself—it's more likely that the g_data.
commands list is either not properly linked or being overwritten or cleared before it gets to execute_commands.

*/
int execute_commands(void)
{
    // Check if g_data.commands is populated
    printf("g_data.commands: %p\n", g_data.commands);
    if (g_data.commands == NULL) {
        printf("No commands to execute.\n");
        return 1;  // Exit early if there are no commands
    }

    printf("here execute_command called\n");

    t_command *cmd = g_data.commands;
    printf("this is command: %s\n", cmd->command_string);  // Debugging

    while (cmd)
    {
        printf("first_token gonna be defined\n");

        // Make sure cmd->token_list is not NULL
        if (cmd->token_list == NULL) {
            printf("No tokens in this command.\n");
        } else {
            printf("cmd->token_list is valid.\n");
        }

        t_token *first_token = cmd->token_list;
        printf("first_token defined\n");

        // Debugging statements
        if (first_token != NULL) {
            printf("First token value: '%s'\n", first_token->value);
            printf("is_command: %d, is_int: %d\n", first_token->is_command, first_token->is_int);
        } else {
            printf("No first token found.\n");
        }

        // Ensure there's a command token to execute
        if (first_token && first_token->is_command)
        {
            // Check if it's an internal command
            if (first_token->is_int == 1)
            {
                printf("Executing internal command: '%s'\n", first_token->value);  // Debugging
                execute_internal_command(cmd);
            }
            else
            {
                printf("Executing external command: '%s'\n", first_token->value);  // Debugging
                execute_external_command(cmd);
            }
        }
        cmd = cmd->next;
    }

    return 1;
}






// int execute_commands(void)
// {
//     printf("here execute_command called\n");
//     t_command *cmd = g_data.commands;

//     while (cmd)
//     {
//         printf("first_token gonna defined\n");
//         t_token *first_token = cmd->token_list;
//         printf("first_token defined\n");
//         // Debugging statements
//         printf("First token value: '%s'\n", first_token->value);
//         printf("is_command: %d, is_int: %d\n", first_token->is_command, first_token->is_int);

//         // Ensure there's a command token to execute
//         if (first_token && first_token->is_command)
//         {
//             // Check if it's an internal command
//             if (first_token->is_int == 1)
//             {
//                 printf("Executing internal command: '%s'\n", first_token->value);  // Debugging
//                 execute_internal_command(cmd);
//             }
//             else
//             {
//                 printf("Executing external command: '%s'\n", first_token->value);  // Debugging
//                 execute_external_command(cmd);
//             }
//         }
//         cmd = cmd->next;
//     }

//     return 1;
// }

