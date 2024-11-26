
#include "../minishell.h"

// void handle_multi_command(t_shell_data *shell, t_redirection *redir, int cmds_index)
// {
//     int **pipes;
//     int cmds_nb;

//     pipes = shell->pipes;
//     cmds_nb = shell->cmds_nb;
//     if (cmds_index != 0 && cmds_index != cmds_nb - 1) // Middle commands
//     {
//         dup2(pipes[cmds_index - 1][0], STDIN_FILENO);
//         dup2(pipes[cmds_index][1], STDOUT_FILENO);
//     }
//     else if (cmds_index == 0)                         // First command
//     {
//         if (!has_redirs(redir, REDIR_OUTPUT) && !has_redirs(redir, REDIR_APPEND))
//             dup2(pipes[cmds_index][1], STDOUT_FILENO);
//     }
//     else if (cmds_index == cmds_nb - 1)                // Last command
//     {
//         if (!has_redirs(redir, REDIR_INPUT))
//             dup2(pipes[cmds_index - 1][0], STDIN_FILENO);
//     }
//     if (cmds_index != 0)
//         close(pipes[cmds_index - 1][0]);
//     if (cmds_index != cmds_nb - 1)
//         close(pipes[cmds_index][1]);
// }

// void handle_single_command(t_redirection *redir)
// {
//     int input_fd;
//     int output_fd;

//     input_fd = 0;
//     output_fd = 0;
//     if (has_redirs(redir, REDIR_INPUT))
//     {
//         input_fd = open_input_file(redir, input_fd);
//         if (input_fd == EXIT_FAILURE)
//             {exit(EXIT_FAILURE);
//             fprintf(stderr, "Error: Failed to handle input redirection.\n");}
//     }
//     if (has_redirs(redir, REDIR_OUTPUT) || has_redirs(redir, REDIR_APPEND))
//     {
//         if (has_redirs(redir, REDIR_APPEND))
//             output_fd = open_append_file(redir, output_fd);
//         else
//             output_fd = open_output_file(redir, output_fd);
//         if (output_fd == EXIT_FAILURE)
//             {exit(EXIT_FAILURE);
//             fprintf(stderr, "Error: Failed to handle input redirection.\n");}
//     }
// }

void if_thereis_redirection(t_shell_data *shell, t_redirection *redir, int cmds_index)
{
    int exit_code;

    exit_code = open_all_files(redir);
    if (exit_code == EXIT_FAILURE)
        return;
    if (!has_redirs(redir, REDIR_INPUT) && cmds_index != 0)
        dup2(shell->pipes[cmds_index - 1][0], STDIN_FILENO);
    
    // Check for output redirection and set up piping if necessary
    if (!has_redirs(redir, REDIR_OUTPUT) && !has_redirs(redir, REDIR_APPEND) && cmds_index != shell->cmds_nb - 1)
        dup2(shell->pipes[cmds_index][1], STDOUT_FILENO);
    // if (shell->cmds_nb > 1)
    //     handle_multi_command(shell, redir, cmds_index);
    // else if (shell->cmds_nb == 1)
    //     handle_single_command(redir);
}
