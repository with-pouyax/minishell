/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 14:12:11 by pghajard          #+#    #+#             */
/*   Updated: 2024/04/24 10:29:03 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t n)
{
	void			*start;
	unsigned char	*ptr;

	start = str;
	ptr = (unsigned char *)str;
	while (n--)
	{
		*ptr = (unsigned char)c;
		ptr++;
	}
	return (start);
}
