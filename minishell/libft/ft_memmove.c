/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:50:36 by pghajard          #+#    #+#             */
/*   Updated: 2024/04/24 10:29:32 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	void			*ptr;
	unsigned char	*destination;
	unsigned char	*source;

	ptr = dest;
	destination = (unsigned char *)dest;
	source = (unsigned char *)src;
	if ((dest == NULL && src == NULL) || dest == src)
		return (dest);
	if (destination > source)
	{
		while (n--)
			destination[n] = source[n];
	}
	else
	{
		i = 0;
		while (i < n)
		{
			destination[i] = source[i];
			i++;
		}
	}
	return (ptr);
}
