/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:52:04 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/21 15:37:55 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*****************************************************************************/
//                        🚀 check_unclosed_quotes 🚀
/*****************************************************************************/
// 🎯 Purpose  :  check if there is unclosed quotes
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  input -> user input
//
// 🔄 Returns   :  1 if we have unclosed quotes 0
//
/*****************************************************************************/
// 💡 Notes:
//     1- using a while loop we iterate over each character in the input.
//     2- if we find a single quote and we are not in a double quote we toggle
//        meaning we change the value of in_single_quote to the opposite value.
//     3- if we find a double quote and we are not in a single quote we toggle
//        meaning we change the value of in_double_quote to the opposite value.
//     4- if in_single_quote or in_double_quote is true we return 1.
/******************************************************************************/

int	check_unclosed_quotes(char *input)
{
	int	in_single_quote;
	int	in_double_quote;
	int	i;

	in_single_quote = 0;
	in_double_quote = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (input[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	return (in_single_quote || in_double_quote);
}

/*
if (c == '\'' && !(*in_double_quote))-> If the char is a single q
and we are not in a double q
*in_single_quote = !(*in_single_quote)-> reverse the value of in_single_q
else if (c == '\"' && !(*in_single_quote))->If the char is a double q and
we are not in a single q
*in_double_quote = !(*in_double_quote)-> reverse the value of in_double_q
*/
void	update_quote_flags(char c, int *in_single_quote, int *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
		*in_single_quote = !(*in_single_quote);
	else if (c == '\"' && !(*in_single_quote))
		*in_double_quote = !(*in_double_quote);
}
