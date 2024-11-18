#include "minishell.h"



void exec_cmd(t_command *cmd, int index, int total_pipes)
{
    pid_t   pid;           // fork()
    int     *pipe_in;
    int     *pipe_out;
    int		status;

    pipe_in = NULL;
    pipe_out = NULL;

    printf("Debug: Starting exec_cmd for command #%d\n", index);
    pid = fork();
    if (pid == -1)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)  // Child process
    {
        printf("Debug: In child process for command #%d\n", index);

        // Redirection if it's not the first command
        if (index > 0)  
        {
            pipe_in = g_data.commands[index - 1].pipes[1];  // Previous command's pipe
            printf("Debug: Redirecting input from pipe_in[0] for command #%d\n", index);
            dup2(pipe_in[0], STDIN_FILENO);  // Redirect input
            close(pipe_in[0]);  // Close read end after dup
            close(pipe_in[1]);  // Close write end
        }

        // Redirection if there is a next pipe
        if (index < total_pipes)
        {
            pipe_out = cmd->pipes[index];
            printf("Debug: Redirecting output to pipe_out[1] for command #%d\n", index);
            dup2(pipe_out[1], STDOUT_FILENO);  // Redirect output
            close(pipe_out[0]);  // Close read end
            close(pipe_out[1]);  // Close write end
        }

        // Execute internal or external commands
        if (cmd->token_list->is_int == 1)
        {
            printf("Debug: Executing internal command for command #%d\n", index);
            execute_internal_commands();
        }
        else
        {
            printf("Debug: Executing external command for command #%d: %s\n", index, cmd->token_list->value);
            execute_external_commands(cmd->token_list);
        }

        printf("Debug: Exiting child process for command #%d\n", index);
        exit(EXIT_SUCCESS);  // Exit child process
    }
    else  // Parent process
    {
        printf("Debug: In parent process, waiting for child #%d\n", index);
        waitpid(pid, &status, 0);
        printf("Debug: Child #%d finished with status %d\n", index, WEXITSTATUS(status));
    }
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

    // printf("Debug: Saved original stdin (%d) and stdout (%d)\n", saved_stdin, saved_stdout);

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
    // printf("Debug: Pipes initialized. cmd->pipe_nb = %d, cmd->cmds_nb = %d\n", cmd->pipe_nb, cmd->cmds_nb);

    // Loop through each command in the command list
    while (cmd)
    {
        // printf("Debug: Executing command #%d: '%s'\n", process_index, cmd->command_string);
        
        // Debugging the process of executing the command
        printf("Debug: Calling exec_cmd() : pipe_nb = %d, Executing command %s\n", cmd->pipe_nb, cmd->command_string);
        exec_cmd(cmd, process_index, cmd->pipe_nb); // Execute the command in a separate process
        
        // Move to the next command in the linked list
        cmd = cmd->next;
        process_index++;  // Increment the process index for the next command
    }

    // Restore original stdin and stdout
    // printf("Debug: Restoring original stdin and stdout\n");
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);

    // Close saved stdin and stdout
    close(saved_stdin);
    close(saved_stdout);

    // printf("Debug: stdin and stdout restored and closed\n");
}
