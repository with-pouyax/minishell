#include "minishell.h"

size_t  ft_strlen(const char *s)
{
    size_t i;

    i = 0;
    while (s && s[i])
        i++;
    return (i);
}

char    *ft_strdup(const char *s1)
{
    size_t  len;
    char    *dup;

    len = ft_strlen(s1);
    dup = malloc(len + 1);
    if (!dup)
        return (NULL);
    ft_memcpy(dup, s1, len);  // Replacing memcpy with ft_memcpy
    dup[len] = '\0';
    return (dup);
}


char    *ft_substr(char const *s, unsigned int start, size_t len)
{
    char    *sub;
    size_t  s_len;

    if (!s)
        return (NULL);
    s_len = ft_strlen(s);
    if (start >= s_len)
        return (ft_strdup(""));
    if (len > s_len - start)
        len = s_len - start;
    sub = malloc(len + 1);
    if (!sub)
        return (NULL);
    ft_memcpy(sub, s + start, len);  // Replacing memcpy with ft_memcpy
    sub[len] = '\0';
    return (sub);
}


char    *ft_strjoin(char const *s1, char const *s2)
{
    size_t  len1;
    size_t  len2;
    char    *join;

    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    join = malloc(len1 + len2 + 1);
    if (!join)
        return (NULL);
    ft_memcpy(join, s1, len1);  // Replacing memcpy with ft_memcpy
    ft_memcpy(join + len1, s2, len2);  // Replacing memcpy with ft_memcpy
    join[len1 + len2] = '\0';
    return (join);
}


char    *ft_strjoin_free(char *s1, char *s2)
{
    char *result;

    result = ft_strjoin(s1, s2);
    free(s1);
    free(s2);
    return (result);
}
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*dst;
	const char	*source;

	if (!dest && !src)
		return (NULL);
	dst = (char *)dest;
	source = (const char *)src;
	i = 0;
	while (i < n)
	{
		dst[i] = source[i];
		i++;
	}
	return (dest);
}