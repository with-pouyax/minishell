#include "../minishell.h"

char	*generate_temp_filename(void)
{
	static int	counter = 0;
	char		*filename;
	char		*counter_str;

	counter_str = ft_itoa(counter++); // [x] Convert the counter to a string
	if (!counter_str)
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
	if (!counter_str)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", counter_str); //[x] Join the prefix and the counter string
	if (!filename)
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
	free(counter_str);
	return (filename);
}



