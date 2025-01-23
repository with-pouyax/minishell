/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_word4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 22:27:33 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/22 22:28:28 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
