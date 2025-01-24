/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_internal_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 09:45:02 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:24:44 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*****************************************************************************/
//   using ft_strcmp() we compare the token with the internal commands
/******************************************************************************/

int	is_internal_command(char *token)
{
	if (!ft_strcmp(token, "echo") || !ft_strcmp(token, "cd")
		|| !ft_strcmp(token, "pwd") || !ft_strcmp(token, "export")
		|| !ft_strcmp(token, "unset") || !ft_strcmp(token, "env")
		|| !ft_strcmp(token, "exit"))
		return (1);
	return (0);
}
