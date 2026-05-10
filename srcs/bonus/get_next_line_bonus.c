/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laraus <laraus@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 17:35:30 by laraus            #+#    #+#             */
/*   Updated: 2026/05/10 17:09:22 by laraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static int	fill_buffer(int fd, char *buff)
{
	int	bytes;

	bytes = read(fd, buff, BUFFER_SIZE);
	if (bytes <= 0)
		return (bytes);
	buff[bytes] = '\0';
	return (bytes);
}

char	*get_next_line(int fd)
{
	static char	buff[FD_MAX][BUFFER_SIZE + 1];
	char		*line;
	int			bytes;

	if (fd < 0 || fd >= FD_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	line = NULL;
	bytes = 1;
	while (bytes > 0)
	{
		if (!buff[fd][0])
			bytes = fill_buffer(fd, buff[fd]);
		if (bytes <= 0)
			break ;
		line = ft_strjoin(line, buff[fd]);
		if (!line)
			return (NULL);
		ft_buffflusher(buff[fd]);
		if (line[ft_strlen(line) - 1] == '\n')
			return (line);
	}
	if (bytes < 0)
		return (free(line), NULL);
	return (line);
}
