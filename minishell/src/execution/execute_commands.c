#include "../minishell.h"

int open_file(char *file_name, int fd, int flag, mode_t )
{}

int open_all_files(t_redir *redir)
{
    int fd_input;
    int fd_output;

    fd_input = -2;
    fd_output = -2;
    while (redir)
    {
        if(redir->type[0] == '<')
        {
            fd_input = open(redir->direction, fd_input, O_RDONLY, 0);
            if (fd_input == -1)
            {
                perror("open file failed");
                return (EXIT_FAILURE);
            }
            dup2(fd_input, STDIN_FILENO);     //redirect input
            close(fd_input); 
        }
        else if (redir->type[0] == '>' && redir->type[1] == '\0' )   // ">"
        {
            fd_output = open(redir->direction, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd_output == -1)
            {
                perror("open file failed");
                return (EXIT_FAILURE);
            }
            dup2(fd_output, STDOUT_FILENO);    //redirect output
            close(fd_output); 
        }
        else if (redir->type[0] == '>' && redir->type[1] == '>')    // ">>"
        {
            fd_output = open(redir->direction, O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (fd_output == -1)
            {
                perror("open");
                return (EXIT_FAILURE);
            }
            dup2(fd_output, STDOUT_FILENO); // Redirect output to append mode
            close(fd_output); 
        }
        
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

void    if_thereis_redirection(t_shell_data *shell, t_redir *redir, int cmds_index)
{
    int **pipes;
    int cmds_nb;
    int exit_code;

    pipes = shell->pipes;
    cmds_nb = shell->cmds_nb;
    exit_code = open_all_files(redir);
    if (exit_code == EXIT_FAILURE)
        return;
    // check the redirections
}

void exec_cmd(t_shell_data *shell ,t_command *cmds, int index)
{
    int saved_stdin;
    int saved_stdout;

    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    // replace_env_var();     // pouya did this part before 
    if_thereis_redirection(shell, cmds->redirs, index);
    // check cmd if is internal or external
    // {   execute_program();  }
    // restore_std();

    // Restore original stdin and stdout
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
    // close pipes
    // exec_parent
    // free
}
