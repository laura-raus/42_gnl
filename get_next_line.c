/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laraus <laraus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 17:29:42 by laraus            #+#    #+#             */
/*   Updated: 2026/05/08 20:44:55 by laraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	buff[BUFFER_SIZE + 1];
	char		*line;
	int			bytes;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	line = NULL;
	bytes = 1;
	while (bytes > 0)
	{
		if (!buff[0])
		{
			bytes = read(fd, buff, BUFFER_SIZE);
			if (bytes < 0)
				return (free(line), NULL);
			buff[bytes] = '\0';
		}
		if (buff[0])
		{
			line = ft_strjoin(line, buff);
			if (!line)
				return (NULL);
			ft_buffflusher(buff);
			if (line[ft_strlen(line) - 1] == '\n')
				return (line);
		}
	}
	return (line);
}
