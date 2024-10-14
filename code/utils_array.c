#include "minishell.h"

char    **ft_split(const char *s, char c)
{
    char    **result;
    size_t  i;
    size_t  j;
    size_t  k;

    if (!s || !(result = malloc(sizeof(char *) * (ft_strlen(s) / 2 + 2))))
        return (NULL);
    i = 0;
    k = 0;
    while (s[i])
    {
        if (s[i] != c)
        {
            j = i;
            while (s[i] && s[i] != c)
                i++;
            result[k++] = ft_substr(s, j, i - j);
        }
        else
            i++;
    }
    result[k] = NULL;
    return (result);
}

void    free_array(char **array)
{
    int i;

    i = 0;
    if (!array)
        return ;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

void    ft_putstr_fd(char *s, int fd)
{
    if (s)
        write(fd, s, ft_strlen(s));
}

void    ft_putchar_fd(char c, int fd)
{
    write(fd, &c, 1);
}

void    ft_putendl_fd(char *s, int fd)
{
    if (s)
    {
        ft_putstr_fd(s, fd);
        ft_putchar_fd('\n', fd);
    }
}
