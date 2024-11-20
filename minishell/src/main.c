#include "minishell.h"




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

/*
The isatty function is used to check if a file descriptor refers to a terminal.
first : Makes sure that the terminal is linked to the STDIN
*/


int main(int argc, char **argv, char **envp)
{
    t_shell_data shell;
	(void)argc;
	(void)argv;

    if (!isatty(STDIN_FILENO))
        exit(EXIT_FAILURE);
    ft_bzero(&shell, sizeof(t_shell_data));
    shell.envp = copy_envp(envp); // okey
	if (!shell.envp)
	{
		ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	init_shell(&shell);
	// setup_signal_handlers(); // Set up signal handlers
        // printf("before handle input");
	handle_input(&shell);
    printf("after handle input");

    // printf("before execution");
    // execution();
    //  printf("after execution");

	cleanup();
	return (0);
}




