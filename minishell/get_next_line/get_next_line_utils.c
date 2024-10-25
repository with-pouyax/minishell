/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyaximac <pouyaximac@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 09:58:55 by pghajard          #+#    #+#             */
/*   Updated: 2024/10/20 20:15:19 by pouyaximac       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	ft_free(char **p)
{
	if (*p)
	{
		free(*p);
		*p = NULL;
	}
}

// size_t	ft_strlen(char *s)
// {
// 	size_t	len;

// 	len = 0;
// 	while (s && s[len])
// 		len++;
// 	return (len);
// }

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

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*destination;
	const unsigned char	*source;

	destination = (unsigned char *)dest;
	source = (unsigned char *)src;
	i = 0;
	if (!dest && !src)
		return (dest);
	while (i < n)
	{
		destination[i] = source[i];
		i++;
	}
	return (dest);
}

// char	*ft_strjoin(char *s1, char *s2)
// {
// 	char	*joined_str;
// 	int		i;
// 	int		j;
// 	int		len;

// 	len = 0;
// 	if (!s1 && !s2)
// 		return (NULL);
// 	if (s1)
// 		len += ft_strlen(s1);
// 	if (s2)
// 		len += ft_strlen(s2);
// 	joined_str = malloc(len + 1);
// 	if (joined_str == NULL)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	while (s1 && s1[j])
// 		joined_str[i++] = s1[j++];
// 	j = 0;
// 	while (s2 && s2[j])
// 		joined_str[i++] = s2[j++];
// 	joined_str[i] = '\0';
// 	return (joined_str);
// }
