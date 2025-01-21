/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:51:47 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/21 15:35:24 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :  skip spaces at beginning of the input
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//     🏷  i -> index of the current character in the input
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- using a while loop we iterate over the input characters.
//     2- if the current character is a whitespace we increment the index.
//     3- if the current character is not a whitespace we stop the loop.
/******************************************************************************/

void	skip_spaces(t_shell_data *shell, int *i)
{
	while (shell->input[*i] && ft_isspace(shell->input[*i]))
		(*i)++;
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷 s1 ->
//     🏷 s2 ->
//
/*****************************************************************************/
// 💡 Notes:
//     1- the difference between this function and ft_strjoin_and_free_first()
//        is that this function frees both s1 and s2 but
//        ft_strjoin_and_free_first() frees only s1 and ft_strjoin() frees
//        neither s1 nor s2.
//
/******************************************************************************/
char	*ft_strjoin_and_free_both(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin_safe(s1, s2);
	if (!joined)
	{
		free(s1);
		free(s2);
		return (NULL);
	}
	free(s1);
	free(s2);
	return (joined);
}
/*****************************************************************************/
//          No explanation needed for this function
/*****************************************************************************/
char	*ft_strjoin_free_both(char *s1, char *s2)
{
	return (ft_strjoin_and_free_both(s1, s2));
}
