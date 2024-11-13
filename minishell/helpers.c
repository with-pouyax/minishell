#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	check_unclosed_quotes(char *input)
{
	int	in_single_quote;
	int	in_double_quote;
	int	i;

	in_single_quote = 0;
	in_double_quote = 0;
	i = 0;
	while (input[i]) // loop through the input string
	{
		if (input[i] == '\'' && !in_double_quote) // if single quote and not in double quote
			in_single_quote = !in_single_quote; // reverse the value of in_single_quote
		else if (input[i] == '\"' && !in_single_quote) // if double quote and not in single quote
			in_double_quote = !in_double_quote; // reverse the value of in_double_quote
		i++;
	}
	return (in_single_quote || in_double_quote); // Return 1 if any of the quotes are 1, 0 otherwise.
}

char	*ft_strdup_free(char *s1)
{
	char	*dup;

	dup = ft_strdup(s1);
	free(s1);
	return (dup);
}

char	*ft_strjoin_safe(const char *s1, const char *s2)
{
	char	*res;
	size_t	s1_len;
	size_t	s2_len;

	s1_len = 0;
	s2_len = 0;
	if (s1)
		s1_len = ft_strlen(s1);
	if (s2)
		s2_len = ft_strlen(s2);
	res = malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!res)
		return (NULL);
	if (s1)
		ft_memcpy(res, s1, s1_len);
	if (s2)
		ft_memcpy(res + s1_len, s2, s2_len);
	res[s1_len + s2_len] = '\0';
	return (res);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin_safe(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

char	*ft_strjoin_and_free_first(char *s1, const char *s2)
{
	char	*joined;

	joined = ft_strjoin_safe(s1, s2);
	free(s1);
	return (joined);
}

char	*ft_strjoin_and_free_second(const char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin_safe(s1, s2);
	free(s2);
	return (joined);
}

char	*ft_strjoin_and_free_both(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin_safe(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

char	*ft_strjoin_free_both(char *s1, char *s2)
{
	return (ft_strjoin_and_free_both(s1, s2));
}



