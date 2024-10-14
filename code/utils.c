#include "minishell.h"

// String Functions

size_t  ft_strlen(const char *s)
{
    size_t i = 0;
    while (s && s[i])
        i++;
    return i;
}

char    *ft_strdup(const char *s1)
{
    size_t  len = ft_strlen(s1);
    char    *dup = malloc(len + 1);
    if (!dup)
        return NULL;
    memcpy(dup, s1, len);
    dup[len] = '\0';
    return dup;
}

char    *ft_substr(char const *s, unsigned int start, size_t len)
{
    char    *sub;
    size_t  s_len;

    if (!s)
        return NULL;
    s_len = ft_strlen(s);
    if (start >= s_len)
        return ft_strdup("");
    if (len > s_len - start)
        len = s_len - start;
    sub = malloc(len + 1);
    if (!sub)
        return NULL;
    memcpy(sub, s + start, len);
    sub[len] = '\0';
    return sub;
}

char    *ft_strjoin(char const *s1, char const *s2)
{
    size_t  len1 = ft_strlen(s1);
    size_t  len2 = ft_strlen(s2);
    char    *join = malloc(len1 + len2 + 1);
    if (!join)
        return NULL;
    memcpy(join, s1, len1);
    memcpy(join + len1, s2, len2);
    join[len1 + len2] = '\0';
    return join;
}

char    *ft_strjoin_free(char *s1, char *s2)
{
    char *result = ft_strjoin(s1, s2);
    free(s1);
    free(s2);
    return result;
}

char    *ft_strjoin3(char *s1, char *s2, char *s3)
{
    char *temp = ft_strjoin(s1, s2);
    char *result = ft_strjoin(temp, s3);
    free(temp);
    return result;
}

char    *ft_strjoin_path(char *s1, char *s2)
{
    char *temp = ft_strjoin(s1, "/");
    char *result = ft_strjoin(temp, s2);
    free(temp);
    return result;
}

int     ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (unsigned char)(*s1) - (unsigned char)(*s2);
}

int     ft_strncmp(const char *s1, const char *s2, size_t n)
{
    size_t i = 0;
    while (i < n && s1[i] && s1[i] == s2[i])
        i++;
    if (i == n)
        return 0;
    return (unsigned char)(s1[i]) - (unsigned char)(s2[i]);
}

int     ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

int     ft_isnumber(char *str)
{
    int i = 0;
    if (str[i] == '+' || str[i] == '-')
        i++;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return 0;
        i++;
    }
    return 1;
}

int     ft_isalnum(int c)
{
    return ((c >= '0' && c <= '9') || 
            (c >= 'A' && c <= 'Z') || 
            (c >= 'a' && c <= 'z'));
}

int     ft_atoi(const char *str)
{
    int res = 0;
    int sign = 1;
    while (*str == ' ' || (*str >= 9 && *str <= 13))
        str++;
    if (*str == '-' || *str == '+')
        sign = (*(str++) == '-') ? -1 : 1;
    while (ft_isdigit(*str))
        res = res * 10 + (*(str++) - '0');
    return res * sign;
}

char    *ft_itoa(int n)
{
    char    *str;
    int     tmp = n;
    int     len = (n <= 0) ? 1 : 0;

    while (tmp && ++len)
        tmp /= 10;
    str = malloc(len + 1);
    if (!str)
        return NULL;
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
    return str;
}

char    *ft_strchr(const char *s, int c)
{
    while (*s)
    {
        if (*s == (char)c)
            return (char *)s;
        s++;
    }
    return (c == '\0') ? (char *)s : NULL;
}

char    **ft_split(const char *s, char c)
{
    char    **result;
    size_t  i = 0, j = 0, k = 0;

    if (!s || !(result = malloc(sizeof(char *) * (ft_strlen(s) / 2 + 2))))
        return NULL;
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
    return result;
}

// Memory Management Functions

void    free_array(char **array)
{
    int i = 0;
    if (!array)
        return;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

void    free_command_list(t_command *cmd_list)
{
    t_command *temp;
    while (cmd_list)
    {
        temp = cmd_list;
        cmd_list = cmd_list->next;
        if (temp->cmd)
            free(temp->cmd);
        if (temp->args)
            free_array(temp->args);
        if (temp->input_fd != STDIN_FILENO)
            close(temp->input_fd);
        if (temp->output_fd != STDOUT_FILENO)
            close(temp->output_fd);
        free(temp);
    }
}

// Output Functions

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
