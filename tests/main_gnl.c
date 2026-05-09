/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_gnl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laraus <laraus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 20:46:37 by laraus            #+#    #+#             */
/*   Updated: 2026/05/09 16:22:11 by laraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

static void	print_line_preview(char *line)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (line[len])
		len++;
	printf("len=%d |", len);
	while (line[i] && i < 60)
	{
		if (line[i] == '\n')
			printf("\\n");
		else
			printf("%c", line[i]);
		i++;
	}
	if (len > 60)
		printf("...");
	printf("|\n");
}

static void	test_header(char *title)
{
	printf("\n");
	printf("============================================================\n");
	printf("%s\n", title);
	printf("============================================================\n");
}

static void	read_file(char *path, int expected_lines)
{
	int		fd;
	char	*line;
	int		count;

	test_header(path);
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		printf("OPEN: FAIL\n");
		return ;
	}
	printf("OPEN: OK\n");

	count = 0;
	line = get_next_line(fd);
	while (line)
	{
		count++;
		printf("[%02d] ", count);
		print_line_preview(line);
		free(line);
		line = get_next_line(fd);
	}

	close(fd);

	printf("\nLINES READ : %d\n", count);
	printf("EXPECTED   : %d\n", expected_lines);

	if (count == expected_lines)
		printf("RESULT     : OK\n");
	else
		printf("RESULT     : FAIL\n");
}

static void	test_invalid_fd(void)
{
	char	*line;

	test_header("INVALID FD");
	line = get_next_line(-1);
	if (!line)
		printf("get_next_line(-1): OK, returned NULL\n");
	else
	{
		printf("get_next_line(-1): FAIL\n");
		free(line);
	}
}

int	main(void)
{
	test_invalid_fd();

	read_file("tests/files/empty.txt", 0);

	read_file("tests/files/one_char.txt", 1);
	read_file("tests/files/one_char_newline.txt", 1);

	read_file("tests/files/no_newline.txt", 1);
	read_file("tests/files/one_newline.txt", 1);

	read_file("tests/files/multiple_lines.txt", 3);
	read_file("tests/files/only_newlines.txt", 4);
	read_file("tests/files/mixed.txt", 4);
	read_file("tests/files/alternating.txt", 5);

	read_file("tests/files/buffer_minus_one.txt", 1);
	read_file("tests/files/buffer_exact.txt", 1);
	read_file("tests/files/buffer_plus_one.txt", 1);
	read_file("tests/files/two_buffer_exact_lines.txt", 2);
	read_file("tests/files/around_buffer_mixed.txt", 3);

	read_file("tests/files/long_line.txt", 1);
	read_file("tests/files/long_no_newline.txt", 1);

	return (0);
}
