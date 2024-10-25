#include "minishell.h"

int is_builtin(char *cmd)
{
    if (ft_strcmp(cmd, "cd") == 0)
        return (1);
    else if (ft_strcmp(cmd, "echo") == 0)
        return (1);
    else if (ft_strcmp(cmd, "pwd") == 0)
        return (1);
    else if (ft_strcmp(cmd, "export") == 0)
        return (1);
    else if (ft_strcmp(cmd, "unset") == 0)
        return (1);
    else if (ft_strcmp(cmd, "env") == 0)
        return (1);
    else if (ft_strcmp(cmd, "exit") == 0)
        return (1);
    return (0);
}

int execute_builtin(t_command *cmd)
{
    if (ft_strcmp(cmd->cmd, "cd") == 0)
        return (builtin_cd(cmd->args));
    else if (ft_strcmp(cmd->cmd, "echo") == 0)
        return (builtin_echo(cmd->args));
    else if (ft_strcmp(cmd->cmd, "pwd") == 0)
        return (builtin_pwd());
    else if (ft_strcmp(cmd->cmd, "export") == 0)
        return (builtin_export(cmd->args));
    else if (ft_strcmp(cmd->cmd, "unset") == 0)
        return (builtin_unset(cmd->args));
    else if (ft_strcmp(cmd->cmd, "env") == 0)
        return (builtin_env());
    else if (ft_strcmp(cmd->cmd, "exit") == 0)
        return (builtin_exit(cmd->args));
    return (1);
}

int builtin_exit(char **args)
{
    int exit_status;

    exit_status = 0;
    if (args[1] != NULL)
    {
        if (ft_isnumber(args[1]))
            exit_status = ft_atoi(args[1]);
        else
        {
            ft_putendl_fd("minishell: exit: numeric argument required", STDERR_FILENO);
            exit_status = 255;
        }
    }
    ft_putendl_fd("exit", STDERR_FILENO);
    cleanup_shell();
    exit(exit_status);
    return (0);
}

int builtin_env(void)
{
    int i;

    i = 0;
    while (g_shell.envp[i])
    {
        ft_putendl_fd(g_shell.envp[i], STDOUT_FILENO);
        i++;
    }
    return (0);
}

int builtin_unset(char **args)
{
    int i;

    i = 1;
    while (args[i])
    {
        unset_env_var(args[i]);
        i++;
    }
    return (0);
}
