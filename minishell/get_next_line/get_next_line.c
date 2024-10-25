/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:16:21 by pghajard          #+#    #+#             */
/*   Updated: 2024/10/14 15:12:41 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_new_line(char *temp)
{
	int	i;

	i = 0;
	while (temp[i])
	{
		if (temp[i] == '\n')
		{
			return (1);
		}
		i++;
	}
	return (0);
}

static char	*fd_reader(int fd, int bytes_read, char *txt)
{
	char	*buffer;
	char	*temp;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (ft_free(&txt), NULL);
	while (bytes_read != 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1) // Error
			return (ft_free(&buffer), ft_free(&txt), NULL);
		buffer[bytes_read] = '\0'; 
		temp = txt;
		txt = ft_strjoin(txt, buffer);
		ft_free(&temp);
		if (!txt) 
			return (ft_free(&buffer), ft_free(&txt), NULL);
		if (ft_new_line(buffer))
		{
			ft_free(&buffer);
			break ;
		}
	}
	ft_free(&buffer);
	return (txt);
}

char	*find_next_line(char *txt)
{
	int		i;
	char	*line;

	i = 0;
	while (txt[i] && txt[i] != '\n')
		i++;
	if (txt[i] == '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
		return (ft_free(&line), NULL);
	ft_memcpy(line, txt, i);
	line[i] = '\0';
	return (line);
}

char	*get_next_line(int fd)
{
	size_t		line_len;
	char		*line;
	static char	*txt;
	int			bytes_read;

	bytes_read = 1;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free(txt), NULL);
	txt = fd_reader(fd, bytes_read, txt);
	if (!txt || !*txt)
		return (ft_free(&txt), NULL);
	line = find_next_line(txt);
	if (!line || !*line)
		return (ft_free(&line), ft_free(&txt), NULL);
	line_len = ft_strlen(line);
	ft_memmove(txt, txt + line_len, ft_strlen(txt + line_len) + 1);
	if (!txt)
		return (ft_free(&line), ft_free(&txt), NULL);
	return (line);
}
