/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_commands_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 09:45:32 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:25:25 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"















/*****************************************************************************/
//             using a while loop iterate through the environment
//             variables and count the number of variables.
/*****************************************************************************/

static int	get_env_size(char **envp)
{
	int	size;

	size = 0;
	while (envp[size])
		size++;
	return (size);
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell ->
//     🏷  env_size ->
//     🏷  envp ->
//
// 🔄 Returns   :  sorted envp
//
/*****************************************************************************/
// 💡 Notes:
//     1- we allocate memory for the sorted environment variables.
//     2- if there is an error allocating memory we print an error message
//        and return NULL.
//     3- we loop through the environment variables using a while loop.
//        a- we duplicate the environment variable from the original
//        environment variables to the sorted environment variables.
//        b- if there is an error duplicating the environment variable we
//           print an error message and using a while loop we free the
//           environment variables array backwards and return NULL.
//        c- we free the sorted_envp array and return NULL.
//     4- now that the loop is done and we have duplicated all the environment
//        variables we set the last element of the sorted environment variables
//        to NULL.
//     5- we return the sorted environment variables.
//
//     we need to duplicate the environment variables because we need to sort
//     them and we don't want to modify the original environment variables.
//     we need to sort the environment variables because we need to print them
//     in a sorted order.
/******************************************************************************/

static char	**duplicate_envp(char **envp, int env_size, t_shell_data *shell)
{
	char	**sorted_envp;
	int		i;

	sorted_envp = malloc(sizeof(char *) * (env_size + 1));
	if (!sorted_envp)
	{
		ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
		return (shell->exit_status = 1, NULL);
	}
	i = 0;
	while (i < env_size)
	{
		sorted_envp[i] = ft_strdup(envp[i]);
		if (!sorted_envp[i])
		{
			ft_putstr_fd("minishell: malloc failed\n", STDERR_FILENO);
			while (--i >= 0)
				free(sorted_envp[i]);
			free(sorted_envp);
			return (shell->exit_status = 1, NULL);
		}
		i++;
	}
	sorted_envp[env_size] = NULL;
	return (sorted_envp);
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  envp ->
//     🏷  env_size ->
//
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1-  using a while loop we iterate through the environment variables.
//         a-  using a nested while loop we iterate through the environment
//             variables starting second element.
//         b- using ft_strcmp() we compare the two environment variables, if
//            they ft_strcmp() returns a positive number means the two strings
//            are not in the correct order because the first string is greater
//            than the second string so we swap the two strings.
//            - here i am actually sorting the environment variables in
//              sorting algorithm called bubble sort.
/******************************************************************************/

static void	sort_envp(char **envp, int env_size)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < env_size - 1)
	{
		j = i + 1;
		while (j < env_size)
		{
			if (ft_strcmp(envp[i], envp[j]) > 0)
			{
				temp = envp[i];
				envp[i] = envp[j];
				envp[j] = temp;
			}
			j++;
		}
		i++;
	}
}

/*****************************************************************************/
//  using a while loop we iterate through the environment variables and print
//  them using printf() with the format "declare -x %s\n".
//  we used declare -x to print the environment variables in the format that
//  bash uses.
/******************************************************************************/

static void	print_and_free_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("declare -x %s\n", envp[i]);
		free(envp[i]);
		i++;
	}
	free(envp);
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1-  we get the size of the environment using get_env_size().
//     2-  we duplicate the environment using duplicate_envp().
//        a- if there is an error making the duplicate we return.
//     3-  we sort the environment using sort_envp().
//     4-  we print the environment using print_and_free_envp().
//     5-  we set the exit status to 0.
/******************************************************************************/

void	print_sorted_env(t_shell_data *shell)
{
	char	**sorted_envp;
	int		env_size;

	env_size = get_env_size(shell->envp);
	sorted_envp = duplicate_envp(shell->envp, env_size, shell);
	if (!sorted_envp)
		return ;
	sort_envp(sorted_envp, env_size);
	print_and_free_envp(sorted_envp);
	shell->exit_status = 0;
}
