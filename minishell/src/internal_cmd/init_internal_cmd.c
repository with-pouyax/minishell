/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_internal_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 09:45:02 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 09:45:14 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_internal_command(char *token)
{
	if (!ft_strcmp(token, "echo") || !ft_strcmp(token, "cd")
		|| !ft_strcmp(token, "pwd") || !ft_strcmp(token, "export")
		|| !ft_strcmp(token, "unset") || !ft_strcmp(token, "env")
		|| !ft_strcmp(token, "exit"))
		return (1);
	return (0);
}
