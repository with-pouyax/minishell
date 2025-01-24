/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 15:09:18 by pghajard          #+#    #+#             */
/*   Updated: 2024/04/24 10:24:06 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	little_len;
	size_t	i;
	size_t	j;

	little_len = 0;
	i = 0;
	if (!*little)
		return ((char *)big);
	while (little[little_len] != '\0')
		little_len++;
	while (i < len && big[i] != '\0')
	{
		j = 0;
		if (big[i] == little[0])
		{
			while (j < little_len && i + j < len && big[i + j] == little[j])
				j++;
			if (j == little_len)
				return ((char *)&big[i]);
		}
		i++;
	}
	return (NULL);
}
