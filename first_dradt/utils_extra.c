#include "minishell.h"

char    *ft_strjoin3(char *s1, char *s2, char *s3)
{
    char *temp;
    char *result;

    temp = ft_strjoin(s1, s2);
    result = ft_strjoin(temp, s3);
    free(temp);
    return (result);
}

char    *ft_strjoin_path(char *s1, char *s2)
{
    char *temp;
    char *result;

    temp = ft_strjoin(s1, "/");
    result = ft_strjoin(temp, s2);
    free(temp);
    return (result);
}

int     ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return ((unsigned char)(*s1) - (unsigned char)(*s2));
}

int     ft_strncmp(const char *s1, const char *s2, size_t n)
{
    size_t i;

    i = 0;
    while (i < n && s1[i] && s1[i] == s2[i])
        i++;
    if (i == n)
        return (0);
    return ((unsigned char)(s1[i]) - (unsigned char)(s2[i]));
}

int     ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}
