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

/******************************************************************************/  
/*                              🚀 copy_envp 🚀                                         
/******************************************************************************/
/* 🎯 Purpose  :   copy envp
/******************************************************************************/

/* 🔹 Parameters:                                                             
/*     🏷  envp  ->  envp is an array of strings that represent the 
		   environment variables in the form of "key=value"                                          


/* 🔄 Returns   :  new_envp

/******************************************************************************/

/* 💡 Notes:                                                                  
/*     1- if envp is NULL (no environment variables) or envp[0] is NULL (no 
	   environment variables) we exit with failure.
	   2- we count the number of environment variables.
	   3- we allocate memory for the new_envp array with the size of the
	   number of environment variables + 1. +1 for the NULL terminator.
	   4- we copy the environment variables to the new_envp array using a
	   while loop.
	   5- if the copy fails we free the memory allocated for the new_envp array
	   using the free_envp_copy function and return NULL.
	   6- we NULL terminate the new_envp array.
	   7- we return the new_envp array.                
/*     
/******************************************************************************/

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
			free_envp_copy(new_envp, i);
			return (NULL);
		}
		i++;
	}
	new_envp[count] = NULL;
	return (new_envp);
}
