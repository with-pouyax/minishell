#include "../minishell.h"

int has_redirs(t_redirection *redir, t_redirection_type type )
{
    while (redir)
    {
        if (redir->type == type)
            return (1);
        redir = redir->next;
    }
    return (0);
}


// int open_file(char *file_name, int fd, int flag, mode_t )
// {}

int open_all_files(t_redirection *redir)
{
    int fd_input;
    int fd_output;

    fd_input = -2;
    fd_output = -2;
    while (redir)
    {
        if(redir->type == REDIR_INPUT) //'>'
            fd_input = open_input_file(redir, fd_input);
        else if (redir->type == REDIR_OUTPUT)   // ">"
            fd_output = open_output_file(redir, fd_output);
        else if (redir->type == REDIR_APPEND)     // ">>"
            fd_output = open_append_file(redir, fd_output);
        redir = redir->next; // Move to the next redirection
    }
    return (EXIT_SUCCESS);
}

/*
if_thereis_redirection() :
    handle the redirection operators in a program. 
    Redirections determine how input and output are handled for 
    commands, such as reading from a file or writing to a file.
    -- fd_i and fd_o :
    represent the file descriptors for input (<) and output (> or >>) files.
    They are initialized to -2, a special value indicating no file is open yet.
*/

void    if_thereis_redirection(t_shell_data *shell, t_redirection *redir, int cmds_index)
{
    int **pipes;
    int cmds_nb;
    int exit_code;

    pipes = shell->pipes;
    cmds_nb = shell->cmds_nb;
    exit_code = open_all_files(redir);
    if (exit_code == EXIT_FAILURE)
        return;
    if (cmds_index != 0 && cmds_index != cmds_nb - 1)   // Set up pipes for commands that aren't the first or last
    {
        dup2(pipes[cmds_index - 1][0], STDIN_FILENO);   // Redirect input from the previous command's output pipe
        dup2(pipes[cmds_index][1], STDOUT_FILENO);      // Redirect output to the next command's input pipe
    }
    else if (cmds_index == 0)                           // First command, only output redirection (if any)
    {
        if (!has_redirs(redir, REDIR_OUTPUT) && !has_redirs(redir, REDIR_APPEND))
            dup2(pipes[cmds_index][1], STDOUT_FILENO);
    }
    else if (cmds_index == cmds_nb - 1)                 // Last command, only input redirection (if any)
    {
        if (!has_redirs(redir, REDIR_INPUT))
            dup2(pipes[cmds_index - 1][0], STDIN_FILENO);
    }
}

void exec_cmd(t_shell_data *shell ,t_command *cmds, int index)
{
    int saved_stdin;
    int saved_stdout;

    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    // process_heredocs(shell);
    // If there was an error while processing heredocs, exit early
    if (shell->error_flag)
    {
        shell->exit_status = 1;
        ft_putstr_fd("minishell: error processing heredocs\n", STDERR_FILENO);
        return;
    }
    // replace_env_var();                       // pouya did this part before 
    if_thereis_redirection(shell, cmds->redirections, index);
    if (shell->exit_status == EXIT_SUCCESS)     //if the previou cmd execute succesfully    //should we check if there is cmd to execute or not??????
    {
        if (shell->commands->token_list->is_int)
            execute_internal_commands(shell);
        else
            execute_external_commands(shell);
    }
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
}


void execution(t_shell_data *shell)
{
    t_command   *cmd;
    int         i;

    i = 0;
    cmd = shell->commands;
    shell->pipes = init_pipes(shell->cmds_nb);
    while (i < shell->cmds_nb)
    {
        exec_cmd(shell, cmd, i);
        cmd = cmd->next;
        i++;
    }
    close_all_pipes(shell->pipes, shell->cmds_nb);
    // exec_parent
    free_pipes(shell->pipes, shell->cmds_nb); 
}

