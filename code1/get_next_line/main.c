#include "get_next_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int     check_line_ends_with_1(char *line)
{
    int len;

    if (!line)
        return (0);
    len = 0;
    while (line[len])
        len++;
    // Check if the last character is '1'
    if (len > 0 && line[len - 1] == '1')
        return (1);
    return (0);
}

int check_line(char *line)
{
    int len;

    if (!line)
        return (0);
    len = 0;
    while (line[len] != '\0')
        len++;
    if (len == 0)
        return (0);
    if (line[len - 1] == '\n')
    {
        if (len > 1 && line[len - 2] == '1')
            return (1);
    }
    else if (line[len - 1] == '1')
        return (1);
    return (0);
}

int main(int argc, char **argv, char **envp)
{
    t_shell shell;

    (void)argc;
    (void)argv;
    init_shell(&shell, envp);
    listen_for_input(&shell);
    cleanup_shell(&shell);

    return 0;
}
