/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:44:44 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/22 21:45:00 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../internal_commands.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷   shell -> our structor
//     🏷   cmd -> our command
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the shell->prev_dir is NULL, it means the OLDPWD environment,
//        meaning the previous directory, is not set, so we print an error
//        message and return 1
//     2- using getcwd(NULL, 0) we get the current directory and store it
//        in the current_dir variable
//     3- using chdir(shell->prev_dir) we change the directory to the previous
//        directory
//        a- if the chdir function returns an error, we print an error message
//           and return 1
//     4- if the current_dir is not NULL, we free the shell->prev_dir and
//        set the shell->prev_dir to the current_dir
//     5- we print the previous directory and set the exit status to 0
//     6- return 0
/******************************************************************************/
int	handle_cd_minus(t_shell_data *shell)
{
	char	*current_dir;

	if (shell->prev_dir == NULL)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		shell->exit_status = 1;
		return (1);
	}
	current_dir = getcwd(NULL, 0);
	if (chdir(shell->prev_dir) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(shell->prev_dir);
		shell->exit_status = 1;
		return (1);
	}
	printf("%s\n", shell->prev_dir);
	if (current_dir)
	{
		free(shell->prev_dir);
		shell->prev_dir = current_dir;
	}
	shell->exit_status = 0;
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷   shell -> our structor
//    🏷   path -> our path
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using getenv_from_envp function we get the value of the HOME
//        environment variable and store it in the home variable
//     2- if the home variable is NULL, it means the HOME environment
//        variable is not set, so we call handle_no_home function
//        and return the value of the function
//     3- if the path is second character is \0, it means the path is "~"
//        so using ft_strdup(home) we duplicate the home variable and
//        store it in the expanded_path variable
//     4- if the path is second character is "/", it means the path is "~/"
//        which means the home directory, so using ft_strjoin(home, &path[1])
//        we join the ~ and / and store it in the expanded_path variable so
//        basically we store ~/ in the expanded_path variable
//     5- if the path is none of the above, it means the path is invalid
//        so we print an error message and return 1
//     6- if there is no expanded_path, it means there is a memory error
//        so we call handle_memory_error function and return the value of
//        the function
//     7- using chdir(expanded_path) we change the directory to the
//        expanded_path and if the chdir function returns an error, we
//        call handle_chdir_error function and return the value of the function
//     8- if the chdir function is successful, we free the expanded_path
//        and set the exit status to 0 and return 0

/******************************************************************************/
int	handle_tilde_path(t_shell_data *shell, char *path)
{
	char	*home;
	char	*expanded_path;

	home = getenv_from_envp(shell, "HOME");
	if (!home)
		return (handle_no_home(shell));
	if (path[1] == '\0')
		expanded_path = ft_strdup(home);
	else if (path[1] == '/')
		expanded_path = ft_strjoin(home, &path[1]);
	else
	{
		write_error(path, "\n");
		shell->exit_status = 1;
		return (1);
	}
	if (!expanded_path)
		return (handle_memory_error(shell));
	if (chdir(expanded_path) != 0)
		return (handle_chdir_error(shell, expanded_path));
	free(expanded_path);
	shell->exit_status = 0;
	return (0);
}