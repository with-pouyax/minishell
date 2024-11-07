#include "minishell.h"

t_shell_data	g_data = {0};

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    /* Initialize the shell */
    g_data.envp = envp; // Save the environment variables in the global data struct
    init_shell();

    /* Parsing Phase */
    handle_input(); // This function handles input parsing and populates g_data.commands

    /* Execution Phase */
    // execute_commands();

    /* Internal Commands Handling Phase */
    handle_internal_commands();
    

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
