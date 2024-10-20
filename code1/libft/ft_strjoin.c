/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:02:09 by pghajard          #+#    #+#             */
/*   Updated: 2024/04/24 10:22:46 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*concat(char *s1, char *s2, char *temp, int total_len)
{
	int	i;
	int	j;
	int	s1_len;

	i = 0;
	j = 0;
	s1_len = ft_strlen(s1);
	while (i < s1_len)
	{
		temp[i] = s1[i];
		i++;
	}
	while (i < total_len - 1)
	{
		temp[i] = s2[j];
		i++;
		j++;
	}
	temp[i] = '\0';
	return (temp);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	total_len;
	size_t	s1_len;
	size_t	s2_len;
	char	*temp;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	total_len = s1_len + s2_len + 1;
	temp = malloc(total_len * sizeof(char));
	if (!temp)
		return (NULL);
	concat((char *)s1, (char *)s2, temp, (int)total_len);
	return (temp);
}
