/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 16:34:31 by pghajard          #+#    #+#             */
/*   Updated: 2024/04/24 10:21:01 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcpy(char *dest, const char *src)
{
	char	*p;

	p = dest;
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
	return (p);
}

static int	num_length(int n)
{
	int	length;

	length = 1;
	while (n != 0)
	{
		n = n / 10;
		if (n != 0)
			length++;
	}
	return (length);
}

static char	*construct_string(int sign, int length, int n)
{
	int		i;
	char	*str;

	i = 0;
	str = (char *)malloc((length + 1) * sizeof(char));
	if (!str)
		return (NULL);
	if (sign == -1)
		str[i++] = '-';
	str[length] = '\0';
	while (length-- > i)
	{
		str[length] = '0' + (n % 10);
		n /= 10;
	}
	return (str);
}

char	*ft_itoa(int n)
{
	int		sign;
	int		length;
	char	*min_str;

	if (n == INT_MIN)
	{
		min_str = (char *)malloc(12 * sizeof(char));
		if (!min_str)
			return (NULL);
		return (ft_strcpy(min_str, "-2147483648"));
	}
	sign = 1;
	if (n < 0)
	{
		sign = -1;
		n = -n;
	}
	length = num_length(n);
	if (sign == -1)
		length++;
	return (construct_string(sign, length, n));
}
