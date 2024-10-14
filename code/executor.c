#include "minishell.h"

void    execute_commands(void)
{
    t_command   *cmd;
    int         pipe_fd[2];
    int         input_fd;
    pid_t       pid;

    cmd = g_shell.cmd_list;
    input_fd = STDIN_FILENO;
    while (cmd)
    {
        if (cmd->next)
        {
            if (pipe(pipe_fd) < 0)
            {
                perror("minishell: pipe");
                return ;
            }
        }
        else
            pipe_fd[1] = STDOUT_FILENO;
        pid = fork();
        if (pid == 0)
            child_process(cmd, input_fd, pipe_fd[1]);
        else if (pid < 0)
            perror("minishell: fork");
        else
        {
            waitpid(pid, &g_shell.last_exit_status, 0);
            if (WIFEXITED(g_shell.last_exit_status))
                g_shell.last_exit_status = WEXITSTATUS(g_shell.last_exit_status);
            if (input_fd != STDIN_FILENO)
                close(input_fd);
            if (pipe_fd[1] != STDOUT_FILENO)
                close(pipe_fd[1]);
            input_fd = pipe_fd[0];
            cmd = cmd->next;
        }
    }
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

char    *get_command_path(char *cmd)
{
    char    **paths;
    char    *path_var;
    char    *cmd_path;
    int     i;

    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        else
            return (NULL);
    }
    path_var = getenv("PATH");
    if (!path_var)
        return (NULL);
    paths = ft_split(path_var, ':');
    i = 0;
    while (paths[i])
    {
        cmd_path = ft_strjoin_path(paths[i], cmd);
        if (access(cmd_path, X_OK) == 0)
        {
            free_array(paths);
            return (cmd_path);
        }
        free(cmd_path);
        i++;
    }
    free_array(paths);
    return (NULL);
}

void    exit_shell(int status)
{
    cleanup_shell();
    exit(status);
}
