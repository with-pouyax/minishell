/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_commands_utils2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:54:30 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/22 21:55:50 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_commands.h"

/*****************************************************************************/
//            No explanation needed for this function
/*****************************************************************************/

int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

/*****************************************************************************/
//            No explanation needed for this function
/*****************************************************************************/

int	is_matching_var(const char *env_var, const char *name, int name_len)
{
	return (ft_strncmp(env_var, name, name_len) == 0 && \
	(env_var[name_len] == '=' || env_var[name_len] == '\0'));
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
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- using a while loop we iterate through the environment variables.
//        a-  if the environment variable does not match the name we copy the
//            environment variable to the new environment variables.
//        b-  if the environment variable matches the name we free the
//            environment variable.
//     2- we set the last element of the new environment variables to NULL.
//     3- we return the number of environment variables.
/******************************************************************************/

int	copy_env_vars(char **new_envp, char **envp, const char *name, int name_len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp[i])
	{
		if (!is_matching_var(envp[i], name, name_len))
		{
			new_envp[j++] = envp[i];
		}
		else
		{
			free(envp[i]);
		}
		i++;
	}
	new_envp[j] = NULL;
	return (j);
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
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- using get_name_length() we get the length of the name.
//     2- using count_env_vars() we get the number of environment variables.
//     3- using allocate_new_envp() we allocate memory for the new environment
//        variables.
//        a- if there is an error allocating memory we print an error message
//           and return.
//     4- using copy_env_vars() we copy the environment variables to the new
//        environment variables.
//     5- we free the old environment variables.
//     6- we set the new environment variables.
//     7- we set the exit status to 0.
/******************************************************************************/

void	handle_remove_env(t_shell_data *shell, const char *name)
{
	int		name_len;
	int		current_size;
	char	**new_envp;

	name_len = get_name_length(name);
	current_size = count_env_vars(shell->envp);
	new_envp = allocate_new_envp(current_size, shell);
	if (!new_envp)
		return ;
	copy_env_vars(new_envp, shell->envp, name, name_len);
	free(shell->envp);
	shell->envp = new_envp;
	shell->exit_status = 0;
}
/*****************************************************************************/
//            No explanation needed for this function
/*****************************************************************************/

void	remove_from_env(t_shell_data *shell, const char *name)
{
	if (!name)
	{
		shell->exit_status = 0;
		return ;
	}
	handle_remove_env(shell, name);
}