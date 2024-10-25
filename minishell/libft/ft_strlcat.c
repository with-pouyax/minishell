/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 11:57:26 by pghajard          #+#    #+#             */
/*   Updated: 2024/04/24 10:29:54 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	char_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static size_t	const_char_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	src_index;

	if ((dst == NULL || src == NULL) && size == 0)
		return (0);
	src_index = 0;
	dst_len = char_strlen(dst);
	src_len = const_char_strlen(src);
	if (dst_len >= size)
		return (size + src_len);
	while (src[src_index] != '\0' && (dst_len + src_index + 1) < size)
	{
		dst[dst_len + src_index] = src[src_index];
		src_index++;
	}
	dst[dst_len + src_index] = '\0';
	return (dst_len + src_len);
}
