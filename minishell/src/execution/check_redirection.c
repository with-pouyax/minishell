
#include "../minishell.h"

// Helper function to check for input redirection and set up piping if necessary
void check_input_redir(t_shell_data *shell, t_redirection *redir, int cmds_index)
{
    if (!has_redirs(redir, REDIR_INPUT) && cmds_index != 0)
        dup2(shell->pipes[cmds_index - 1][0], STDIN_FILENO);
}

// Helper function to check for output redirection and set up piping if necessary
void check_output_redir(t_shell_data *shell, t_redirection *redir, int cmds_index)
{
    if (!has_redirs(redir, REDIR_OUTPUT) && !has_redirs(redir, REDIR_APPEND) && cmds_index != shell->cmds_nb - 1)
        dup2(shell->pipes[cmds_index][1], STDOUT_FILENO);
}


void set_redirection_pipes(t_shell_data *shell, t_redirection *redir, int cmds_index)
{
    int exit_code;

    exit_code = open_all_files(shell, redir);
    if (exit_code == EXIT_FAILURE)
        return;
    check_input_redir(shell, redir, cmds_index);
    check_output_redir(shell, redir, cmds_index);
    shell->exit_status = EXIT_SUCCESS;
}

