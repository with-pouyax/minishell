#include "../minishell.h"



// void exec_cmd(t_command *cmd)
// {
//     int saved_stdin;
//     int saved_stdout;
//     // int cmd_index;

//     saved_stdin = dup(STDIN_FILENO);
//     saved_stdout = dup(STDOUT_FILENO);
//     cmd_index = cmd->index;
//     // replace_env_var();
//     // redirection_pipe(cmd_index);
//     // check cmd if is internal or external
//     // {   execute_program();  }
//     // restore_std();

//     // Restore original stdin and stdout
//     dup2(saved_stdin, STDIN_FILENO);
//     dup2(saved_stdout, STDOUT_FILENO);
//     close(saved_stdin);
//     close(saved_stdout);

// }


// void execution()
// {
//     t_command   *cmd;
//     int         i;

//     i = 0;
//     cmd = g_data.commands;
//     cmd->pipes = init_pipes(cmd->cmds_nb);
//     while (i < cmd->cmds_nb)
//     {
//         exec_cmd(cmd);
//         cmd = cmd->next;
//         i++;
//     }
//     // close pipes
//     // exec_parent
//     // free
// }
