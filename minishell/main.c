#include "minishell.h"

t_shell_data	g_data = {0};

int debug_print_commands(void)
{
    t_command *cmd = g_data.commands;
    t_token *token;
    int command_count = 0;

    if (!cmd)
    {
        printf("No commands to execute.\n");
        return (0);
    }

    while (cmd)
    {
        command_count++;
        printf("Executing Command #%d: %s\n", cmd->index, cmd->command_string);
        token = cmd->token_list;
        while (token)
        {
            printf("execute_commands: Function called.\n");
            printf("  Token #%d: '%s'\n", token->index, token->value);
            
            // Detailed Token Properties
            if (token->is_command)
                printf("    - Type: Command\n");
            if (token->is_builtin)
                printf("    - Builtin: Yes\n");
            if (token->is_flag)
                printf("    - Type: Flag\n");
            if (token->is_argument)
                printf("    - Type: Argument\n");
            if (token->is_operator)
                printf("    - Type: Operator\n");
            if (token->is_file)
                printf("    - Type: File\n");
            if (token->is_variable_expansion)
                printf("    - Type: Variable Expansion\n");
            if (token->is_env_assignment)
                printf("    - Type: Environment Assignment\n");
            if (token->is_int)
                printf("    - Internal Command: Yes\n");
            if (token->var_not_found)
                printf("    - Variable Not Found: Yes\n");
            if (token->wrong_operator)
                printf("    - Warning: Invalid Operator Detected\n");
            if (token->is_heredoc)
            {
                printf("    - Type: Heredoc\n");
                printf("      * Delimiter: '%s'\n", token->heredoc_delimiter);
                printf("      * File: '%s'\n", token->heredoc_file);
            }

            token = token->next;
        }
        printf("\n"); // Separator between commands
        cmd = cmd->next;
    }

    printf("Total Commands Executed: %d\n", command_count);
    return (0);
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    /* Initialize the shell */
    g_data.envp = envp; // Save the environment variables in the global data struct
    init_shell();

    /* Parsing Phase */
    handle_input(); // This function handles input parsing and populates g_data.commands


    

    /* Cleanup before exiting */
    cleanup();

    return (0);
}



/* Internal Commands Handling Function Stub */
int handle_internal_commands(void)
{
    // TODO: Implement internal command handling logic
    return (1);
}
