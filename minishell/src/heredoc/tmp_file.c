#include "../minishell.h"

char	*generate_temp_filename(void)
{
	static int	counter = 0;
	char		*filename;
	char		*counter_str;

	counter_str = ft_itoa(counter++); // Convert the counter to a string
	if (!counter_str)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", counter_str); // Join the prefix and the counter string
	free(counter_str);
	return (filename);
}



