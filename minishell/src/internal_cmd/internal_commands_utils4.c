/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_commands_utils4.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:54:56 by pghajard          #+#    #+#             */
/*   Updated: 2025/01/22 18:11:58 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

/*****************************************************************************/
//      No explanation needed for this function
/*****************************************************************************/

void	finalize_new_envp(char **new_envp, char *new_var, int current_size)
{
	new_envp[current_size] = new_var;
	new_envp[current_size + 1] = NULL;
}
