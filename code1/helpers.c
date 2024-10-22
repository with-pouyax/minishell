/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by yourname          #+#    #+#             */
/*   Updated: 2024/10/21 15:50:47 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Check for unclosed quotes
int	check_unclosed_quotes(char *input)
{
	int	in_single_quote; // it will be 1 at the end if we have an unclosed single quote
	int	in_double_quote; // it will be 1 at the end if we have an unclosed double quote
	int	i;

	in_single_quote = 0;
	in_double_quote = 0;
	i = 0;
	while (input[i]) // loop through the input
	{
		if (input[i] == '\'' && !in_double_quote) // if we find a single quote and we are not in a double quote
			in_single_quote = !in_single_quote; // we reverse the value of in_single_quote
		else if (input[i] == '\"' && !in_single_quote) // if we find a double quote and we are not in a single quote
			in_double_quote = !in_double_quote; // we reverse the value of in_double_quote
		i++; // we go to the next character
	}
	return (in_single_quote || in_double_quote); // if any of these are 1, we return 1, else we return 0
}

// Function to duplicate a string and free the original
char	*ft_strdup_free(char *s1)
{
	char	*dup;

	dup = ft_strdup(s1);
	free(s1);
	return (dup);
}

// Function to join two strings and free both
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}
