/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_commands_utils4.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:55:01 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/22 21:57:07 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_commands.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  str
//     🏷  envp
//     🏷  key_len
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1-  using a while loop we iterate through the environment variables.
//         a-  if the environment variable matches the key we return the index.
//     2-  if we don't find the key we return -1.
//
/******************************************************************************/

int	find_env_index(char **envp, const char *key, int key_len)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell
//     🏷  index
//     🏷  str
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- we free the environment variable at the index.
//     2- we duplicate the new environment variable and store it at the index.
//     3- if there is an error duplicating the new environment variable we print
//        an error message and set the exit status to 1.
//
/******************************************************************************/
void	replace_env_var(t_shell_data *shell, int index, const char *str)
{
	free(shell->envp[index]);
	shell->envp[index] = ft_strdup(str);
	if (!shell->envp[index])
	{
		ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
		shell->exit_status = 1;
	}
}
/*****************************************************************************/
//   duplicate the new environment variable and store it in new_var.
/*****************************************************************************/
char	*duplicate_new_var(const char *str, t_shell_data *shell)
{
	char	*new_var;

	new_var = ft_strdup(str);
	if (!new_var)
	{
		ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
		shell->exit_status = 1;
	}
	return (new_var);
}

/*****************************************************************************/
//    we allocate memory for the new environment variables.
/*****************************************************************************/

char	**allocate_new_envp(int current_size, t_shell_data *shell)
{
	char	**new_envp;

	new_envp = malloc(sizeof(char *) * (current_size + 2));
	if (!new_envp)
	{
		ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
		shell->exit_status = 1;
	}
	return (new_envp);
}
/*****************************************************************************/
// using a while loop we copy the existing environment variables to the new
// environment variables.
/*****************************************************************************/
void	copy_existing_envp(char **new_envp, char **envp, int current_size)
{
	int	i;

	i = 0;
	while (i < current_size)
	{
		new_envp[i] = envp[i];
		i++;
	}
}