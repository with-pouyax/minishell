/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_commands_utils3.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:54:56 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:25:34 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*****************************************************************************/
//      No explanation needed for this function
/*****************************************************************************/

void	finalize_new_envp(char **new_envp, char *new_var, int current_size)
{
	new_envp[current_size] = new_var;
	new_envp[current_size + 1] = NULL;
}
/*****************************************************************************/
//  free the old environment variables
/*****************************************************************************/
void	replace_envp(t_shell_data *shell, char **new_envp)
{
	free(shell->envp);
	shell->envp = new_envp;
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell
//     🏷  str
//     🏷  current_size
//     🏷  new_var
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- using allocate_new_envp() we allocate memory for the new environment
//        variables.
//     2- if there is an error allocating memory we print an error message and
//        return.
//     3- using copy_existing_envp() we copy the existing environment variables
//        to the new environment variables.
//     4- using finalize_new_envp() we finalize the new environment variables.
//     5- using replace_envp() we replace the environment variables.
//     6- we set the exit status to 0.
//
//
/******************************************************************************/
static void	handle_new_env_var(t_shell_data *shell, const char *str, \
int current_size, char *new_var)
{
	char	**new_envp;

	(void) str;
	new_envp = allocate_new_envp(current_size, shell);
	if (!new_envp)
	{
		free(new_var);
		return ;
	}
	copy_existing_envp(new_envp, shell->envp, current_size);
	finalize_new_envp(new_envp, new_var, current_size);
	replace_envp(shell, new_envp);
	shell->exit_status = 0;
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell
//     🏷  str
//     🏷  current_size
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- using duplicate_new_var() we duplicate the new environment variable
//        and store it in new_var.
//        a- if there is an error duplicating the new environment variable we
//           print an error message and return.
//     2- using allocate_new_envp() we allocate memory for the new environment
//        variables.
//
//
/******************************************************************************/

void	add_new_env_var(t_shell_data *shell, const char *str, int current_size)
{
	char	*new_var;

	new_var = duplicate_new_var(str, shell);
	if (!new_var)
		return ;
	handle_new_env_var(shell, str, current_size, new_var);
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  str
//     🏷  shell
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1-  we parse the key from the string using parse_key().
//     2-  if there is an error parsing the key we print an error message
//         and return.
//     3-  we get the length of the key using ft_strlen() and store it in
//         key_len.
//     4-  we find the index of the environment variable using find_env_index()
//         a-  if the index is not -1 means the environment variable exists
//             I-  we replace the environment variable using replace_env_var().
//         b-  if the index is -1 means the environment variable does not exist
//			    I-  we add the new environment variable using add_new_env_var()
//	   5-  we free the key.
//
//
/******************************************************************************/

void	add_to_env(t_shell_data *shell, const char *str)
{
	char	*key;
	int		key_len;
	int		index;
	int		size;

	key = parse_key(str);
	if (!key)
	{
		ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
		shell->exit_status = 1;
		return ;
	}
	key_len = ft_strlen(key);
	index = find_env_index(shell->envp, key, key_len);
	if (index != -1)
		replace_env_var(shell, index, str);
	else
	{
		size = 0;
		while (shell->envp[size])
			size++;
		add_new_env_var(shell, str, size);
	}
	free(key);
}

