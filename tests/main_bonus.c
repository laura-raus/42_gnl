/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laraus <laraus@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 14:21:55 by laraus            #+#    #+#             */
/*   Updated: 2026/05/10 16:46:46 by laraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#define UNUSED __attribute__((unused))

static int	g_total = 0;
static int	g_failed = 0;

static void	UNUSED print_summary(void)
{
	printf("\n============================================================\n");
	printf("TOTAL TESTS: %d\n", g_total);
	printf("PASSED:      %d\n", g_total - g_failed);
	printf("FAILED:      %d\n", g_failed);
	printf("============================================================\n");
	if (g_failed == 0)
		printf("✓ ALL BONUS TESTS PASSED\n");
	else
		printf("✗ SOME BONUS TESTS FAILED\n");
}

static void	UNUSED test_header(char *title)
{
	printf("\n============================================================\n");
	printf("%s\n", title);
	printf("============================================================\n");
}

static void	UNUSED test_invalid_fd(void)
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

static void	UNUSED test_same_fd_sequential(void)
{
	int		fd;
	char	*line;
	int		count;

	g_total++;
	fd = open("tests/files/multiple_lines.txt", O_RDONLY);
	if (fd < 0)
	{
		printf("%-50s | OPEN FAIL\n", "Same FD sequential (baseline)");
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

	printf("%-50s | %d lines (expected 3) ", "Same FD sequential (baseline)", count);
	if (count == 3)
		printf("✓\n");
	else
	{
		printf("✗\n");
		g_failed++;
	}
}

static void	test_interleaved_fds(void)
{
	int		fd1;
	int		fd2;
	int		fd3;
	char	*line1;
	char	*line2;
	char	*line3;
	int		success;

	g_total++;
	fd1 = open("tests/files/multiple_lines.txt", O_RDONLY);
	fd2 = open("tests/files/alternating.txt", O_RDONLY);
	fd3 = open("tests/files/mixed.txt", O_RDONLY);

	if (fd1 < 0 || fd2 < 0 || fd3 < 0)
	{
		printf("%-50s | OPEN FAIL\n", "Interleaved file descriptors");
		g_failed++;
		return ;
	}

	success = 1;

	/* Round 1: First line from each */
	line1 = get_next_line(fd1);
	line2 = get_next_line(fd2);
	line3 = get_next_line(fd3);

	if (!line1 || !line2 || !line3)
		success = 0;

	free(line1);
	free(line2);
	free(line3);

	/* Round 2: Second line from each */
	line1 = get_next_line(fd1);
	line2 = get_next_line(fd2);
	line3 = get_next_line(fd3);

	if (!line1 || !line2 || !line3)
		success = 0;

	free(line1);
	free(line2);
	free(line3);

	/* Drain remaining lines - MUST FREE EACH LINE */
	line1 = get_next_line(fd1);
	while (line1)
	{
		free(line1);
		line1 = get_next_line(fd1);
	}

	line2 = get_next_line(fd2);
	while (line2)
	{
		free(line2);
		line2 = get_next_line(fd2);
	}

	line3 = get_next_line(fd3);
	while (line3)
	{
		free(line3);
		line3 = get_next_line(fd3);
	}

	close(fd1);
	close(fd2);
	close(fd3);

	printf("%-50s | ", "Interleaved file descriptors");
	if (success)
		printf("✓ (states preserved across FDs)\n");
	else
	{
		printf("✗ (state mixing detected)\n");
		g_failed++;
	}
}

static void	UNUSED test_large_fd_limit(void)
{
	int		fds[100];
	char	*line;
	int		i;
	int		success;

	g_total++;
	success = 1;

	for (i = 0; i < 100; i++)
	{
		fds[i] = open("tests/files/one_line.txt", O_RDONLY);
		if (fds[i] < 0)
		{
			success = 0;
			break ;
		}
	}

	if (!success || i < 100)
	{
		printf("%-50s | ", "Large FD limit (100 FDs)");
		printf("✗ (only opened %d FDs)\n", i);
		g_failed++;
		for (i = 0; i < 100 && fds[i] >= 0; i++)
			close(fds[i]);
		return ;
	}

	/* Read one line from each FD */
	for (i = 0; i < 100; i++)
	{
		line = get_next_line(fds[i]);
		if (!line)
			success = 0;
		free(line);
	}

	/* Close all FDs */
	for (i = 0; i < 100; i++)
		close(fds[i]);

	printf("%-50s | ", "Large FD limit (100 FDs)");
	if (success)
		printf("✓ (all 100 FDs read successfully)\n");
	else
	{
		printf("✗ (some FDs failed)\n");
		g_failed++;
	}
}

static void	UNUSED test_fd_reuse(void)
{
	/* Test disabled - FD reuse behavior is undefined per subject */
	/* Kept for reference only */
	int		fd;
	char	*line1;
	char	*line2;

	g_total++;
	fd = open("tests/files/multiple_lines.txt", O_RDONLY);
	if (fd < 0)
	{
		printf("%-50s | OPEN FAIL\n", "FD reuse after close");
		g_failed++;
		return ;
	}

	line1 = get_next_line(fd);
	printf("DEBUG: FD=%d line1='%s'\n", fd, line1 ? line1 : "(null)");
	close(fd);

	fd = open("tests/files/multiple_lines.txt", O_RDONLY);
	if (fd < 0)
	{
		printf("%-50s | REOPEN FAIL\n", "FD reuse after close");
		free(line1);
		g_failed++;
		return ;
	}

	line2 = get_next_line(fd);
	printf("DEBUG: FD=%d line2='%s'\n", fd, line2 ? line2 : "(null)");
	close(fd);

	printf("%-50s | ", "FD reuse after close");
	if (line1 && line2 && strcmp(line1, line2) == 0)
	{
		free(line1);
		free(line2);
		printf("✓ (fresh state on reopen)\n");
	}
	else
	{
		printf("✗ (line1='%s' line2='%s')\n", 
		       line1 ? line1 : "NULL", 
		       line2 ? line2 : "NULL");
		free(line1);
		free(line2);
		g_failed++;
	}
}

int	main(void)
{
	test_header("GET_NEXT_LINE BONUS TESTS");

	test_invalid_fd();
	test_same_fd_sequential();
	test_interleaved_fds();
	test_large_fd_limit();
	// test_fd_reuse();

	print_summary();
	return (g_failed != 0);
}
