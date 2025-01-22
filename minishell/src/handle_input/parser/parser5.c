/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:18:54 by pghajard          #+#    #+#             */
/*   Updated: 2025/01/22 13:19:07 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*****************************************************************************/
//       No explanation needed for this function
/*****************************************************************************/

int	initialize_word(char **word)
{
	*word = ft_strdup("");
	if (!*word)
		return (1);
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :  if the current character is a space or an operator
//                character return 1
/*****************************************************************************/

int	should_break(const t_shell_data *shell, char c)
{
	if (!shell->in_single_quote && !shell->in_double_quote)
	{
		if (ft_isspace(c) || is_operator_char(c))
			return (1);
	}
	return (0);
}

