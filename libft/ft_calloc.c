/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 11:21:22 by pghajard          #+#    #+#             */
/*   Updated: 2024/04/24 10:19:04 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*memory;
	size_t	total_size;
	size_t	i;

	total_size = nmemb * size;
	i = 0;
	if (nmemb > 0 && total_size / nmemb != size)
	{
		return (NULL);
	}
	memory = malloc(total_size);
	if (!memory)
	{
		return (NULL);
	}
	while (i < total_size)
	{
		((char *)memory)[i] = 0;
		i++;
	}
	return (memory);
}
