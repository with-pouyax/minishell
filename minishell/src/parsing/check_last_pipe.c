/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_last_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:36:07 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 10:36:32 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*****************************************************************************/
//                        🚀 check_leading_pipe 🚀
/*****************************************************************************/
// 🎯 Purpose  :  check if the input start with pipe
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  input -> user input
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we check if the input is NULL, that means the input is empty.
//        a- if the input is empty we return 0.
//     2- we iterate over the input and skip the whitespaces.
//     3- if we find a pipe at the beginning we return 1.
//     4- if we don't find a pipe at the beginning we return 0.
/******************************************************************************/

int	check_leading_pipe(char *input)
{
	int	i;

	if (!input)
		return (0);
	i = 0;
	while (input[i] && ft_isspace(input[i]))
		i++;
	if (input[i] == '|')
		return (1);
	return (0);
}

/*****************************************************************************/
//                        🚀 check_leading_pipe 🚀
/*****************************************************************************/
// 🎯 Purpose  :  check if the input end with pipe
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  input -> user input
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1- we check if the input is NULL, that means the input is empty.
//        a- if the input is empty we return 0.
//     2- we find the length of the input - 1.
//     3- we iterate backwards over the input and skip the whitespaces.
//     4- if we find a pipe at the end we return 1.
//     5- if we don't find a pipe at the end we return 0.
/******************************************************************************/

int	check_trailing_pipe(char *input)
{
	int	i;

	if (!input)
		return (0);
	i = ft_strlen(input) - 1;
	while (i >= 0 && ft_isspace(input[i]))
		i--;
	if (i >= 0 && input[i] == '|')
		return (1);
	return (0);
}