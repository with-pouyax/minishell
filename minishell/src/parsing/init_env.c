#include "../minishell.h"


char **copy_envp(char **envp)
{
    int i;
    int count;
    char **new_envp;

    count = 0;
    i = 0;
    if (!envp || !envp[0])
        exit(EXIT_FAILURE);
    while (envp[count]) // Count the number of environment variables, envp is an array of strings
        count++;
    new_envp = malloc(sizeof(char *) * (count + 1)); // Allocate memory for the environment variables
    if (!new_envp)
        return (NULL);
    while (i < count) // loop through the environment variables
    {
        new_envp[i] = ft_strdup(envp[i]); //Copy each environment variable to new_envp, which is an array of strings
        if (!new_envp[i])
        {
            for (int j = 0; j < i; j++)
                free(new_envp[j]);
            free(new_envp);
            return NULL;
        }
        i++;
    }
    new_envp[count] = NULL; // Set the last element of new_envp to NULL
    return (new_envp);
}
