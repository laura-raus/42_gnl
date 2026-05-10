/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_gnl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laraus <laraus@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 20:46:37 by laraus            #+#    #+#             */
/*   Updated: 2026/05/10 16:46:52 by laraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

static int	g_total = 0;
static int	g_failed = 0;

static void	print_summary(void)
{
	printf("\n============================================================\n");
	printf("TOTAL TESTS: %d\n", g_total);
	printf("PASSED:      %d\n", g_total - g_failed);
	printf("FAILED:      %d\n", g_failed);
	printf("============================================================\n");
	if (g_failed == 0)
		printf("✓ ALL TESTS PASSED\n");
	else
		printf("✗ SOME TESTS FAILED\n");
}

static void	test_header(char *title)
{
	printf("\n============================================================\n");
	printf("%s\n", title);
	printf("============================================================\n");
}

static void	check_file(char *path, int expected_lines)
{
	int		fd;
	char	*line;
	int		count;

	g_total++;
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		printf("%-50s | OPEN FAIL\n", path);
		g_failed++;
		return ;
	}
	count = 0;
	line = get_next_line(fd);
	while (line)
	{
		count++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	printf("%-50s | %d lines (expected %d) ", path, count, expected_lines);
	if (count == expected_lines)
		printf("✓\n");
	else
	{
		printf("✗\n");
		g_failed++;
	}
}

static void	test_invalid_fd(void)
{
	char	*line;

	g_total++;
	line = get_next_line(-1);
	printf("%-50s | ", "Invalid file descriptor (-1)");
	if (!line)
		printf("✓ (returned NULL)\n");
	else
	{
		printf("✗ (returned non-NULL)\n");
		free(line);
		g_failed++;
	}
}

static void	test_stdin(void)
{
	int		pipefd[2];
	char	*line;
	int		saved_stdin;

	g_total++;
	printf("%-50s | ", "Reading from stdin (pipe test)");
	saved_stdin = dup(STDIN_FILENO);
	if (pipe(pipefd) == -1)
	{
		printf("✗ (pipe creation failed)\n");
		g_failed++;
		return ;
	}
	write(pipefd[1], "hello\nworld\n", 12);
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	line = get_next_line(STDIN_FILENO);
	if (line && line[0] == 'h' && line[1] == 'e')
	{
		free(line);
		line = get_next_line(STDIN_FILENO);
		if (line && line[0] == 'w')
		{
			free(line);
			printf("✓\n");
		}
		else
		{
			printf("✗ (second line failed)\n");
			g_failed++;
		}
	}
	else
	{
		printf("✗ (first line failed)\n");
		g_failed++;
	}
	dup2(saved_stdin, STDIN_FILENO);
	close(pipefd[0]);
	close(saved_stdin);
}

int	main(void)
{
	test_header("GET_NEXT_LINE TESTS");
	
	test_invalid_fd();
	test_stdin();
	
	check_file("tests/files/empty.txt", 0);
	check_file("tests/files/one_char.txt", 1);
	check_file("tests/files/one_char_newline.txt", 1);
	check_file("tests/files/no_newline.txt", 1);
	check_file("tests/files/one_newline.txt", 1);
	check_file("tests/files/multiple_lines.txt", 3);
	check_file("tests/files/only_newlines.txt", 4);
	check_file("tests/files/mixed.txt", 4);
	check_file("tests/files/alternating.txt", 5);
	check_file("tests/files/buffer_minus_one.txt", 1);
	check_file("tests/files/buffer_exact.txt", 1);
	check_file("tests/files/buffer_plus_one.txt", 1);
	check_file("tests/files/two_buffer_exact_lines.txt", 2);
	check_file("tests/files/around_buffer_mixed.txt", 3);
	check_file("tests/files/long_line.txt", 1);
	check_file("tests/files/long_no_newline.txt", 1);
	
	print_summary();
	return (g_failed != 0);
}
