#include "minishell.h"

void    load_history(void)
{
    char    *history_file;
    char    *home;
    int     result;

    home = getenv("HOME");
    if (!home)
        home = ".";
    history_file = ft_strjoin(home, "/.minishell_history");
    result = read_history(history_file);
    if (result != 0 && errno != ENOENT)
        perror("minishell: load_history");
    free(history_file);
}

void    save_history(void)
{
    char    *history_file;
    char    *home;
    int     result;

    home = getenv("HOME");
    if (!home)
        home = ".";
    history_file = ft_strjoin(home, "/.minishell_history");
    result = write_history(history_file);
    if (result != 0)
        perror("minishell: save_history");
    free(history_file);
}

void    add_to_history(char *input)
{
    if (input && *input)
        add_history(input);
}
