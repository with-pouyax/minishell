/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:16:49 by pghajard          #+#    #+#             */
/*   Updated: 2024/04/24 10:08:36 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	char	*p;

	i = 0;
	p = NULL;
	while (s[i])
	{
		if (s[i] == ((char)c))
		{
			p = (char *)&s[i];
		}
		i++;
	}
	if (s[i] == ((char)c))
		p = (char *)&s[i];
	return (p);
}
