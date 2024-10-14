#include "minishell.h"

int     ft_isnumber(char *str)
{
    int i;

    i = 0;
    if (str[i] == '+' || str[i] == '-')
        i++;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

int     ft_isalnum(int c)
{
    return ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z'));
}

int     ft_atoi(const char *str)
{
    int res;
    int sign;

    res = 0;
    sign = 1;
    while (*str == ' ' || (*str >= 9 && *str <= 13))
        str++;
    if (*str == '-' || *str == '+')
        sign = (*(str++) == '-') ? -1 : 1;
    while (ft_isdigit(*str))
        res = res * 10 + (*(str++) - '0');
    return (res * sign);
}

char    *ft_itoa(int n)
{
    char    *str;
    int     tmp;
    int     len;

    tmp = n;
    len = (n <= 0) ? 1 : 0;
    while (tmp && ++len)
        tmp /= 10;
    str = malloc(len + 1);
    if (!str)
        return (NULL);
    str[len] = '\0';
    if (n == 0)
        str[0] = '0';
    if (n < 0)
        str[0] = '-';
    while (n)
    {
        str[--len] = '0' + ((n < 0) ? -(n % 10) : (n % 10));
        n /= 10;
    }
    return (str);
}

char    *ft_strchr(const char *s, int c)
{
    while (*s)
    {
        if (*s == (char)c)
            return ((char *)s);
        s++;
    }
    return ((c == '\0') ? (char *)s : NULL);
}
