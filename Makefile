# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: laraus <laraus@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/06 17:26:50 by laraus            #+#    #+#              #
#    Updated: 2026/05/09 16:17:45 by laraus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = gnl_test
BONUS_NAME = gnl_bonus_test

CC = cc
CFLAGS = -Wall -Wextra -Werror

BUFFER_SIZE ?= 42

INCLUDES = -Iincludes
BUFFER_FLAG = -D BUFFER_SIZE=$(BUFFER_SIZE)

SRC_DIR = srcs/
BONUS_DIR = srcs/bonus/
TEST_DIR = tests/

MANDATORY = \
	$(SRC_DIR)get_next_line.c \
	$(SRC_DIR)get_next_line_utils.c

BONUS = \
	$(BONUS_DIR)get_next_line_bonus.c \
	$(BONUS_DIR)get_next_line_utils_bonus.c

TEST_MANDATORY = $(TEST_DIR)main_gnl.c
TEST_BONUS = $(TEST_DIR)main_bonus.c

OBJS = $(MANDATORY:.c=.o)
BONUS_OBJS = $(BONUS:.c=.o)

all: test

%.o: %.c
	$(CC) $(CFLAGS) $(BUFFER_FLAG) $(INCLUDES) -c $< -o $@

generate_tests:
	BUFFER_SIZE=$(BUFFER_SIZE) ./tests/scripts/generate_tests.sh

test: generate_tests $(NAME)
	./$(NAME)

$(NAME): $(OBJS) $(TEST_MANDATORY)
	$(CC) $(CFLAGS) $(BUFFER_FLAG) $(INCLUDES) \
		$(TEST_MANDATORY) $(OBJS) -o $(NAME)

bonus: generate_tests $(BONUS_NAME)
	./$(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS) $(TEST_BONUS)
	$(CC) $(CFLAGS) $(BUFFER_FLAG) $(INCLUDES) \
		$(TEST_BONUS) $(BONUS_OBJS) -o $(BONUS_NAME)

clean:
	rm -f $(OBJS) $(BONUS_OBJS)

clean_tests:
	rm -f $(NAME) $(BONUS_NAME)

fclean: clean clean_tests

re: fclean all

valgrind: generate_tests $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

valgrind_bonus: generate_tests $(BONUS_NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(BONUS_NAME)

.PHONY: all generate_tests test bonus clean clean_tests fclean re \
	valgrind valgrind_bonus
