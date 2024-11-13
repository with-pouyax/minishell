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
char **copy_envp(char **envp)
{
    int i;
    int count;
    char **new_envp;

    // Count number of environment variables
    count = 0;
    while (envp[count])
        count++;

    // Allocate new envp
    new_envp = malloc(sizeof(char *) * (count + 1));
    if (!new_envp)
        return NULL;

    // Copy environment variables
    for (i = 0; i < count; i++)
    {
        new_envp[i] = ft_strdup(envp[i]);
        if (!new_envp[i])
        {
            // In case of failure, free already allocated strings
            for (int j = 0; j < i; j++)
                free(new_envp[j]);
            free(new_envp);
            return NULL;
        }
    }
    new_envp[count] = NULL;

    return new_envp;
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	/* Initialize the shell */
	g_data.envp = copy_envp(envp); // Copy the environment variables
	if (!g_data.envp)
	{
		ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	init_shell();

	setup_signal_handlers(); // Set up signal handlers

	/* Handle input and execute commands */
	handle_input();

	/* Cleanup before exiting */
	cleanup();

	return (0);
}




