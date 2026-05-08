/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laraus <laraus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 20:46:37 by laraus            #+#    #+#             */
/*   Updated: 2026/05/08 20:53:45 by laraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

static void	test_file(char *filename)
{
	int		fd;
	char	*line;
	int		i;

	printf("\n=== %s ===\n", filename);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return ;
	}
	i = 1;
	line = get_next_line(fd);
	while (line)
	{
		printf("[%02d] |%s|", i, line);
		if (line[0] && line[ft_strlen(line) - 1] != '\n')
			printf("\n");
		free(line);
		i++;
		line = get_next_line(fd);
	}
	close(fd);
}

int	main(void)
{
	char	*line;

	test_file("tests/empty.txt");
	test_file("tests/one_char.txt");
	test_file("tests/no_newline.txt");
	test_file("tests/one_newline.txt");
	test_file("tests/multiple_lines.txt");
	test_file("tests/only_newlines.txt");
	test_file("tests/long_line.txt");
	test_file("tests/mixed.txt");

	printf("\n=== invalid fd ===\n");
    line = get_next_line(42);
    if (!line)
    printf("fd 42: NULL\n");
    else
    printf("fd 42: %s\n", line);
    free(line);
    printf("\n=== invalid fd 42 ===\n");
    
	line = get_next_line(-1);
	if (!line)
		printf("invalid fd: NULL\n");
	else
		printf("invalid fd: %s\n", line);
	free(line);
	return (0);
}
