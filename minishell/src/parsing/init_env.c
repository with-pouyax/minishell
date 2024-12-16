#include "../minishell.h"

static void	free_envp_copy(char **new_envp, int i)
{
	int	j;

	j = 0;
	while (j < i) 								// loop through the new_envp array of strings and free the memory allocated for each string
	{
		free(new_envp[j]);					    //freeing the memory allocated for the strings in the new_envp array
		j++;
	}
	free(new_envp);
}

char	**copy_envp(char **envp)
{
	int	i;
	int	count;
	char	**new_envp;                						//new_envp is an array of strings, which is a copy of the envp array of strings

	count = 0;
	i = 0;
	if (!envp || !envp[0])
		exit(EXIT_FAILURE);
	while (envp[count])				   					   //counting the number of strings in the envp array
		count++;
	new_envp = malloc(sizeof(char *) * (count + 1));       //allocating memory for the new_envp array of strings
	if (!new_envp)
		return (NULL);
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])									//if the allocation of memory for the new_envp[i] string fails
		{
			free_envp_copy(new_envp, i); 					//freeing the memory allocated for the strings in the new_envp array
			return (NULL);
		}
		i++;
	}
	new_envp[count] = NULL; 								//setting the last element of the new_envp array to NULL
	return (new_envp); 										//returning the new_envp array of strings
}
