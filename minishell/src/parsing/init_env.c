#include "../minishell.h"

static void	free_envp_copy(char **new_envp, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		free(new_envp[j]);
		j++;
	}
	free(new_envp);
}

char	**copy_envp(char **envp)
{
	int	i;
	int	count;
	char	**new_envp;

	count = 0;
	i = 0;
	if (!envp || !envp[0])
		exit(EXIT_FAILURE);
	while (envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			ree_envp_copy(new_envp, i);
			return (NULL);
		}
		i++;
	}
	new_envp[count] = NULL;
	return (new_envp);
}
