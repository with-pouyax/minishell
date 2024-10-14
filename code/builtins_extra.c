#include "minishell.h"

int builtin_export(char **args)
{
    int i;

    if (args[1] == NULL)
        print_sorted_env();
    else
    {
        i = 1;
        while (args[i])
        {
            if (set_env_var(args[i]) != 0)
                return (1);
            i++;
        }
    }
    return (0);
}

int builtin_pwd(void)
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        ft_putendl_fd(cwd, STDOUT_FILENO);
        return (0);
    }
    else
    {
        perror("minishell: pwd");
        return (1);
    }
}

int builtin_echo(char **args)
{
    int i;
    int newline;

    i = 1;
    newline = 1;
    if (args[1] && ft_strcmp(args[1], "-n") == 0)
    {
        newline = 0;
        i++;
    }
    while (args[i])
    {
        ft_putstr_fd(args[i], STDOUT_FILENO);
        if (args[i + 1])
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }
    if (newline)
        ft_putchar_fd('\n', STDOUT_FILENO);
    return (0);
}

int builtin_cd(char **args)
{
    char *path;

    if (args[1] == NULL)
        path = getenv("HOME");
    else
        path = args[1];
    if (chdir(path) != 0)
    {
        perror("minishell: cd");
        return (1);
    }
    return (0);
}
