/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:00:16 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/21 16:29:31 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/builtins.h"


/*****************************************************************************/
// 🎯 Purpose  : Validates the given path string.
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷   path ->
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the path is NULL we return 0.
//     2- using a while loop we iterate over the path string.
//        a- if we find "..$" in the path we print an error message and return 1.
//     3- if we finish the loop without finding "..$" we return 0.
/******************************************************************************/
int	validate_path(const char *path)
{
	int	i;

	if (!path)
		return (0);

	i = 0;
	while (path[i + 2])
	{
		if (path[i] == '.' && path[i + 1] == '.' && path[i + 2] == '$')
		{
			ft_putstr_fd((char *)path, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
			return (1);
		}
		i++;
	}
	return (0);
}