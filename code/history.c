#include "minishell.h"

void    load_history(void)
{
    char    *history_file;
    char    *home;
    int     result;

    home = getenv("HOME"); /* what getenv does is it returns the value of the environment variable passed to it, in this case HOME */
    if (!home) /* if the environment variable HOME is not set, we set it to the current directory */
        home = ".";
    history_file = ft_strjoin(home, "/.minishell_history"); /* here we are creating a string that contains the path to the history file, here in strjoin
    the first argument is the home directory and the second argument is the file name, so what is happening here is that we are concatenating the home 
    directory with the file name to get the full path to the history file and storing it in history_file */
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
