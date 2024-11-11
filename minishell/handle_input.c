#include "minishell.h"

int	handle_unclosed_quotes(void)
{
	char	*additional_input;

	additional_input = readline("quote> ");
	if (!additional_input)
	{
		free(g_data.input);
		return (0);
	}
	if (append_additional_input(additional_input) == -1)
	{
		free(additional_input);
		return (-1);
	}
	free(additional_input);
	return (1);
}

int append_additional_input(char *additional_input)
{
    g_data.input = ft_strjoin_and_free_first(g_data.input, "\n");
    if (!g_data.input)
    {
        ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
        return (-1);
    }
    g_data.input = ft_strjoin_and_free_first(g_data.input, additional_input);
    if (!g_data.input)
    {
        ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
        return (-1);
    }
    g_data.full_input = ft_strjoin_and_free_first(g_data.full_input, "\n");
    if (!g_data.full_input)
    {
        ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
        return (-1);
    }
    g_data.full_input = ft_strjoin_and_free_first(g_data.full_input, additional_input);
    if (!g_data.full_input)
    {
        ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
        return (-1);
    }
    return (0);
}



void	process_input(void)
{
	preprocess_input();
}

void	handle_input(void)
{
	int	ret;

	while (1)
	{
		g_data.input = readline(PROMPT);
		if (!g_data.input)
			break ;
		if (ft_strlen(g_data.input) > MAX_INPUT_LENGTH)
		{
			ft_putstr_fd("minishell: input too long\n", STDERR_FILENO);
			free(g_data.input);
			continue ;
		}
		g_data.full_input = ft_strdup(g_data.input);
		if (check_unclosed_quotes(g_data.input))
		{
			ret = handle_unclosed_quotes();
			if (ret <= 0)
			{
				cleanup();
				break ;
			}
		}
		process_input();

		free(g_data.input);
		g_data.input = NULL;
		free(g_data.full_input);
		g_data.full_input = NULL;
	}
	rl_clear_history();

	printf("handle_input: Exiting input loop.\n");
}
