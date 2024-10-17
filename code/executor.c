#include "minishell.h"

int execute_command(t_command *cmd)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // Child process
        execve(cmd->path, cmd->args, g_shell.envp);
        exit(EXIT_FAILURE); // If execve fails
    }
    else
    {
        // Parent process waits for child to finish
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            g_shell.last_exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            g_shell.last_exit_status = 128 + WTERMSIG(status);
    }
    return g_shell.last_exit_status;
}



void    child_process(t_command *cmd, int input_fd, int output_fd)
{
    if (input_fd != STDIN_FILENO)
    {
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
    if (output_fd != STDOUT_FILENO)
    {
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }
    if (cmd->input_fd != STDIN_FILENO)
    {
        dup2(cmd->input_fd, STDIN_FILENO);
        close(cmd->input_fd);
    }
    if (cmd->output_fd != STDOUT_FILENO)
    {
        dup2(cmd->output_fd, STDOUT_FILENO);
        close(cmd->output_fd);
    }
    if (is_builtin(cmd->cmd))
    {
        execute_builtin(cmd);
        exit(g_shell.last_exit_status);
    }
    else
    {
        execute_external(cmd);
        exit(g_shell.last_exit_status);
    }
}

void    execute_external(t_command *cmd)
{
    char    *cmd_path;

    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    cmd_path = get_command_path(cmd->cmd);
    if (!cmd_path)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(cmd->cmd, STDERR_FILENO);
        ft_putendl_fd(": command not found", STDERR_FILENO);
        exit(127);
    }
    execve(cmd_path, cmd->args, g_shell.envp);
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    perror(cmd->cmd);
    exit(1);
}

char *get_command_path(char *cmd)
{
    char *path_env = getenv("PATH");
    char **paths = ft_split(path_env, ':');
    char *full_path;
    int i;

    // Iterate over all directories in PATH
    for (i = 0; paths[i]; i++)
    {
        full_path = ft_strjoin_path(paths[i], cmd); // Join path with command name
        if (access(full_path, X_OK) == 0) // Check if command exists and is executable
        {
            free_array(paths);
            return full_path;
        }
        free(full_path);
    }
    free_array(paths);
    return NULL; // Command not found
}
