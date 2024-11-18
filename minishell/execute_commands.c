#include "minishell.h"



void exec_cmd(t_command *cmd, int index, int total_pipes)
{
    pid_t   pid;           //fork()
    int     *pipe_in;
    int     *pipe_out;
    int		status;

    pipe_in = NULL;
    pipe_out = NULL;
    pid = fork();
    if (pid == -1)
        exit(EXIT_FAILURE);
    if (pid == 0)
    {
        // redirection if it's not a first cmd
        if (index > 0)   //if it's not a first cmd
        {
            pipe_in = g_data.commands[index - 1].pipes[1];  // gets the write end of the previous command's pipe
            dup2(pipe_in[0], STDIN_FILENO);                 // Redirect input from previous pipe (read end)        }
            close(pipe_in[0]);  // Close after dup
            close(pipe_in[1]);
        }
        // redirection (if there is a next pipe)
        if (index < total_pipes)
        {
            pipe_out = cmd->pipes[index];
            dup2(pipe_out[1], STDOUT_FILENO);
            close(pipe_out[0]);
            close(pipe_out[1]);
        }
         if (cmd->token_list->is_int == 1)
         {
            execute_internal_commands();
         }
         else
         {
            execute_external_commands(cmd->token_list);
         }
        exit(EXIT_SUCCESS);  // Ensure the child process exits properly
    }
    else
        waitpid(pid, &status, 0);   // Parent process: Wait for the child to finish
}



void execution()
{
    t_command *cmd;
    int process_index;
    int saved_stdin;
    int saved_stdout;

    process_index = 0;

    // Save original stdin and stdout to restore later
    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);

    printf("Debug: Saved original stdin (%d) and stdout (%d)\n", saved_stdin, saved_stdout);

    cmd = g_data.commands;

    // If no commands, print debug and exit
    if (!cmd) 
    {
        printf("Debug: No commands to execute, exiting execution function.\n");
        return;
    }

    // Initialize pipes for each command if there are multiple
    cmd->pipe_nb = cmd->cmds_nb - 1;  // Set number of pipes based on command count
    cmd->pipes = init_pipes(cmd->cmds_nb);  // Initialize pipes
    printf("Debug: Pipes initialized. cmd->pipe_nb = %d, cmd->cmds_nb = %d\n", cmd->pipe_nb, cmd->cmds_nb);

    // Loop through each command in the command list
    while (cmd)
    {
        printf("Debug: Executing command #%d: '%s'\n", process_index, cmd->command_string);
        
        // Debugging the process of executing the command
        printf("Debug: Calling exec_cmd() with process_index = %d, pipe_nb = %d\n", process_index, cmd->pipe_nb);
        exec_cmd(cmd, process_index, cmd->pipe_nb); // Execute the command in a separate process
        
        // Move to the next command in the linked list
        cmd = cmd->next;
        process_index++;  // Increment the process index for the next command
    }

    // Restore original stdin and stdout
    printf("Debug: Restoring original stdin and stdout\n");
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);

    // Close saved stdin and stdout
    close(saved_stdin);
    close(saved_stdout);

    printf("Debug: stdin and stdout restored and closed\n");
}
