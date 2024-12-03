#include "../minishell.h"

void	skip_spaces(t_shell_data *shell, int *i)
{
	while (shell->input[*i] && ft_isspace(shell->input[*i]))
		(*i)++;
}

// char	*ft_strjoin_and_free_second(const char *s1, char *s2)
// {
// 	char	*joined;

// 	joined = ft_strjoin_safe(s1, s2);
// 	free(s2);
// 	return (joined);
// }

char	*ft_strjoin_and_free_both(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin_safe(s1, s2);
	if (!joined)
	{
		free(s1);
		free(s2);
		return (NULL);
	}
	free(s1);
	free(s2);
	return (joined);
}

char	*ft_strjoin_free_both(char *s1, char *s2)
{
	return (ft_strjoin_and_free_both(s1, s2));
}
