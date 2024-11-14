#include "minishell.h"



// int execute_commands(void)
// {
//     // +-----------------------+
//     // |      EXECUTION PHASE  |
//     // +-----------------------+
//     // Ensure debug_print_commands is declared in the header file or define it here
//     //debug_print_commands(); // ##debug print
//     // +-----------------------+
//     // |  END EXECUTION PHASE  |
//     // +-----------------------+

//     // TODO: Add actual command execution logic here.

//     return (0);
// }
int **init_pipes(int cmds_nb)
{
    int **pipes;
    int i;

    i = 0;
    if (cmds_nb == 1)
        return (NULL);
    pipes = malloc(sizeof(int *) * (cmds_nb - 1));
    if (!pipes)
        exit (EXIT_FAILURE);
    while (i < (cmds_nb - 1))
    {
        pipes[i] = malloc(sizeof(int) * 2); //2 for read and write
        if (!pipes[i])
            exit (EXIT_FAILURE);
        if (pipe(pipes[i]) == -1)
            exit (EXIT_FAILURE);
        i++;
    }
    return (pipes);
}

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


void    execution()
{
    t_command	*cmd;
    int         process_index;
    int         saved_stdin;
    int         saved_stdout;

    process_index = 0;
    saved_stdin = dup(STDIN_FILENO); // Save original stdin and stdout
    saved_stdout = dup(STDOUT_FILENO);
	cmd = g_data.commands;
    cmd->pipe_nb = cmd->cmds_nb - 1;
    cmd->pipes = init_pipes(cmd->cmds_nb);
	while (cmd)
	{
        exec_cmd(cmd, process_index, cmd->pipe_nb); // Execute command in a separate process
        cmd = cmd->next;
        process_index++;
    }

    // Restore original stdin and stdout
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);

    // Close saved stdin and stdout
    close(saved_stdin);
    close(saved_stdout);
}
