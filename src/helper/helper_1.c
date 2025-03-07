/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:51:23 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:22:04 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strdup_free(char *s1)
{
	char	*dup;

	dup = ft_strdup(s1);
	free(s1);
	return (dup);
}

/*****************************************************************************/
//              No explanation needed for this function
/*****************************************************************************/

char	*ft_strjoin_safe(const char *s1, const char *s2)
{
	char	*res;
	size_t	s1_len;
	size_t	s2_len;

	s1_len = 0;
	s2_len = 0;
	if (s1)
		s1_len = ft_strlen(s1);
	if (s2)
		s2_len = ft_strlen(s2);
	res = malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!res)
	{
		write(STDERR_FILENO, "minishell: malloc failed\n", 25);
		return (NULL);
	}
	if (s1)
		ft_memcpy(res, s1, s1_len);
	if (s2)
		ft_memcpy(res + s1_len, s2, s2_len);
	res[s1_len + s2_len] = '\0';
	return (res);
}
