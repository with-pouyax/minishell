/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:53:07 by pghajard          #+#    #+#             */
/*   Updated: 2024/04/24 10:24:13 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_copy(const char *s1, char *trimmed, int start, int trimmed_len)
{
	int	i;

	i = 0;
	while (i < trimmed_len)
	{
		trimmed[i] = s1[start + i];
		i++;
	}
	trimmed[i] = '\0';
	return (trimmed);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	s1_len;
	int		start;
	int		end;
	size_t	trim_len;
	char	*trimmed;

	start = 0;
	if (!s1 || !set)
		return (NULL);
	s1_len = ft_strlen(s1);
	end = s1_len - 1;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	while (end > start && ft_strchr(set, s1[end]))
		end--;
	if (end < start)
		trim_len = 0;
	else
		trim_len = end - start + 1;
	trimmed = (char *)malloc((trim_len + 1) * sizeof(char));
	if (!trimmed)
		return (NULL);
	return (ft_copy(s1, trimmed, start, trim_len));
}
