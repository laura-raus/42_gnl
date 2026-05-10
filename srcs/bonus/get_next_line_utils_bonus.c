/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laraus <laraus@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 17:35:12 by laraus            #+#    #+#             */
/*   Updated: 2026/05/10 16:47:21 by laraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	copy_buffer(char *str, char *s2, size_t i)
{
	size_t	j;
	size_t	len;

	j = 0;
	len = ft_strlen(s2);
	while (s2[j] && j < len)
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] == '\n')
			return (i + 1);
		i++;
	}
	return (i);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	size_t	i;

	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
	{
		free(s1);
		return (NULL);
	}
	i = 0;
	while (s1 && s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	copy_buffer(str, s2, i);
	free(s1);
	return (str);
}

void	ft_buffflusher(char *buff)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < BUFFER_SIZE && buff[i] && buff[i] != '\n')
		i++;
	if (buff[i] == '\n')
		i++;
	while (i < BUFFER_SIZE && buff[i])
	{
		buff[j] = buff[i];
		buff[i] = '\0';
		i++;
		j++;
	}
	while (j < BUFFER_SIZE)
		buff[j++] = '\0';
}
