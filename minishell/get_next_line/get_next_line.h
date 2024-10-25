/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyaximac <pouyaximac@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 09:05:00 by pghajard          #+#    #+#             */
/*   Updated: 2024/10/20 20:14:59 by pouyaximac       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/stat.h>
# include "../libft/libft.h"
// test
//# include <fcntl.h>
//# include <stdio.h>

//size_t	ft_strlen(char *s);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
//char	*ft_strjoin(char *s1, char *s2);
int		ft_new_line(char *temp);
void	ft_free(char **p);
char	*find_next_line(char *txt);
char	*get_next_line(int fd);

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10

# endif

#endif
