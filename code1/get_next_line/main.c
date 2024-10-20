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

int main(void)
{
    int     fd;
    char    *line;
    int     line_num;

    fd = open("test.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return (1);
    }
    line_num = 1;
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("Line %d: %s", line_num, line); // Print the line
        if (!check_line(line))
        {
            printf("Error: Line %d does not end with '1'.\n", line_num);
            free(line);
            get_next_line(-1);
            close(fd);
            return (1);
        }
        free(line);
        line_num++;
    }
    close(fd);
    return (0);
}